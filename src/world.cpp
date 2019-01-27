// Header
#include "world.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

// Same as static in c, local to compilation unit
namespace
{
	const size_t MAX_TURTLES = 15;
	const size_t MAX_FISH = 5;
	const size_t TURTLE_DELAY_MS = 2000;
	const size_t FISH_DELAY_MS = 5000;

	namespace
	{
		void glfw_err_cb(int error, const char* desc)
		{
			fprintf(stderr, "%d: %s", error, desc);
		}
	}
}

World::World() :
	m_points(0),
	m_next_turtle_spawn(0.f),
	m_next_fish_spawn(0.f)
{
	// Seeding rng with random device
	m_rng = std::default_random_engine(std::random_device()());
}

World::~World()
{

}

// World initialization
bool World::init(vec2 screen)
{
	//-------------------------------------------------------------------------
	// GLFW / OGL Initialization
	// Core Opengl 3.
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);
	m_window = glfwCreateWindow((int)screen.x, (int)screen.y, "A1 Assignment", nullptr, nullptr);
	if (m_window == nullptr)
		return false;

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1); // vsync

	// Load OpenGL function pointers
	gl3w_init();

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
	glfwSetKeyCallback(m_window, key_redirect);
	glfwSetCursorPosCallback(m_window, cursor_pos_redirect);

	// Create a frame buffer
	m_frame_buffer = 0;
	glGenFramebuffers(1, &m_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

	// Initialize the screen texture
	m_screen_tex.create_from_screen(m_window);

	//-------------------------------------------------------------------------
	// Loading music and sounds
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL Audio");
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		fprintf(stderr, "Failed to open audio device");
		return false;
	}

	m_background_music = Mix_LoadMUS(audio_path("music.wav"));
	m_salmon_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
	m_salmon_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));

	if (m_background_music == nullptr || m_salmon_dead_sound == nullptr || m_salmon_eat_sound == nullptr)
	{
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav"),
			audio_path("salmon_dead.wav"),
			audio_path("salmon_eat.wav"));
		return false;
	}

	// Playing background music undefinitely
	Mix_PlayMusic(m_background_music, -1);

	fprintf(stderr, "Loaded music\n");

	m_current_speed = 1.f;

	if (m_salmon.init() && m_water.init())
	{
		Wall wall;
		if (wall.init())
		{
			wall.set_position({ (screen.x / 2),  140 });
			m_walls.emplace_back(wall);
			return true;
		}
		fprintf(stderr, "Failed to spawn wall\n");
	}
	return false;
}

// Releases all the associated resources
void World::destroy()
{
	glDeleteFramebuffers(1, &m_frame_buffer);

	if (m_background_music != nullptr)
		Mix_FreeMusic(m_background_music);
	if (m_salmon_dead_sound != nullptr)
		Mix_FreeChunk(m_salmon_dead_sound);
	if (m_salmon_eat_sound != nullptr)
		Mix_FreeChunk(m_salmon_eat_sound);

	Mix_CloseAudio();

	m_salmon.destroy();
	for (auto& turtle : m_turtles)
		turtle.destroy();
	for (auto& fish : m_fish)
		fish.destroy();
	for (auto& missile : m_missiles)
		missile.destroy();
	m_turtles.clear();
	m_fish.clear();
	m_missiles.clear();
	glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms)
{
	int w, h;
        glfwGetFramebufferSize(m_window, &w, &h);
	vec2 screen = { (float)w, (float)h };

	// Checking Salmon - Turtle collisions
	for (const auto& turtle : m_turtles)
	{
		if (m_salmon.collides_with(turtle))
		{
			if (m_salmon.is_alive()) {
				Mix_PlayChannel(-1, m_salmon_dead_sound, 0);
				m_water.set_salmon_dead();
			}
			m_salmon.kill();
			break;
		}
	}

	// Checking Salmon - Fish collisions
	auto fish_it = m_fish.begin();
	while (fish_it != m_fish.end())
	{
		if (m_salmon.is_alive() && m_salmon.collides_with(*fish_it))
		{
			fish_it = m_fish.erase(fish_it);
			m_salmon.light_up();
			Mix_PlayChannel(-1, m_salmon_eat_sound, 0);
			++m_points;
		}
		else
			++fish_it;
	}

	// check missile - turtle collisions
	auto missile_it = m_missiles.begin();
	while (missile_it != m_missiles.end())
	{
		// TODO: this is O(n^2), there's gotta be a better way
		if (m_missiles.empty()) {
			break;
		}
		auto turtle_it = m_turtles.begin();

		while (turtle_it != m_turtles.end()) {
			if (m_turtles.empty()) {
				break;
			}
			if ((*missile_it).collides_with(*turtle_it))
			{
				std::cout << "missile collided with turtle" << std::endl;
				missile_it = m_missiles.erase(missile_it);
				turtle_it = m_turtles.erase(turtle_it);
				break;
			} else
			{
				turtle_it++;
			}
		}
		missile_it++;
	}


	// Updating all entities, making the turtle and fish
	// faster based on current
	m_salmon.update(elapsed_ms, m_walls);
	for (auto& turtle : m_turtles)
		turtle.update(elapsed_ms * m_current_speed);
	for (auto& fish : m_fish)
		fish.update(elapsed_ms * m_current_speed);

	for (auto& missile : m_missiles)
		missile.update(elapsed_ms * m_current_speed);

	// Removing out of screen turtles
	auto turtle_it = m_turtles.begin();
	while (turtle_it != m_turtles.end())
	{
		float w = turtle_it->get_bounding_box().x / 2;
		if (turtle_it->get_position().x + w < 0.f)
		{
			turtle_it = m_turtles.erase(turtle_it);
			continue;
		}

		++turtle_it;
	}

	// Removing out of screen fish
	fish_it = m_fish.begin();
	while (fish_it != m_fish.end())
	{
		float w = fish_it->get_bounding_box().x / 2;
		if (fish_it->get_position().x + w < 0.f)
		{
			fish_it = m_fish.erase(fish_it);
			continue;
		}

		++fish_it;
	}

	// Removing out of screen missiles (code re-used from the fish check code)
	missile_it = m_missiles.begin();
	while (missile_it != m_missiles.end())
	{
		float w = missile_it->get_bounding_box().x / 2;
		if (missile_it->get_position().x + w < 0.f)
		{
			std::cout << "removed missile" << std::endl;
			missile_it = m_missiles.erase(missile_it);
			continue;
		}

		++missile_it;
	}

	// Spawning new turtles
	m_next_turtle_spawn -= elapsed_ms * m_current_speed;
	if (m_turtles.size() <= MAX_TURTLES && m_next_turtle_spawn < 0.f)
	{
		if (!spawn_turtle())
			return false;

		Turtle& new_turtle = m_turtles.back();

		// Setting random initial position
		new_turtle.set_position({ screen.x + 150, 50 + m_dist(m_rng) * (screen.y - 100) });

		// Next spawn
		m_next_turtle_spawn = (TURTLE_DELAY_MS / 2) + m_dist(m_rng) * (TURTLE_DELAY_MS/2);
	}

	// Spawning new fish
	m_next_fish_spawn -= elapsed_ms * m_current_speed;
	if (m_fish.size() <= MAX_FISH && m_next_fish_spawn < 0.f)
	{
		if (!spawn_fish())
			return false;
		Fish& new_fish = m_fish.back();

		new_fish.set_position({ screen.x + 150, 50 + m_dist(m_rng) *  (screen.y - 100) });

		m_next_fish_spawn = (FISH_DELAY_MS / 2) + m_dist(m_rng) * (FISH_DELAY_MS / 2);
	}

	// If salmon is dead, restart the game after the fading animation
	if (!m_salmon.is_alive() &&
		m_water.get_salmon_dead_time() > 5) {
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		m_salmon.destroy();
		m_salmon.init();
		m_turtles.clear();
		m_fish.clear();
		m_water.reset_salmon_dead_time();
		m_current_speed = 1.f;
	}

	return true;
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void World::draw()
{
	// Clearing error buffer
	gl_flush_errors();

	// Getting size of window
	int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

	// Updating window title with points
	std::stringstream title_ss;
	title_ss << "Points: " << m_points;
	glfwSetWindowTitle(m_window, title_ss.str().c_str());

	/////////////////////////////////////
	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	const float clear_color[3] = { 0.3f, 0.3f, 0.8f };
	glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Fake projection matrix, scales with respect to window coordinates
	// PS: 1.f / w in [1][1] is correct.. do you know why ? (:
	float left = 0.f;// *-0.5;
	float top = 0.f;// (float)h * -0.5;
	float right = (float)w;// *0.5;
	float bottom = (float)h;// *0.5;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	mat3 projection_2D{ { sx, 0.f, 0.f },{ 0.f, sy, 0.f },{ tx, ty, 1.f } };

	// Drawing entities
	for (auto& turtle : m_turtles)
		turtle.draw(projection_2D);
	for (auto& fish : m_fish)
		fish.draw(projection_2D);
	for (auto& missile : m_missiles)
		missile.draw(projection_2D);
	for (auto& wall : m_walls)
		wall.draw(projection_2D);
	m_salmon.draw(projection_2D);

	/////////////////////
	// Truely render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(0, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_screen_tex.id);

	m_water.draw(projection_2D);

	//////////////////
	// Presenting
	glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over()const
{
	return glfwWindowShouldClose(m_window);
}

// Creates a new turtle and if successfull adds it to the list of turtles
bool World::spawn_turtle()
{
	Turtle turtle;
	if (turtle.init())
	{
		m_turtles.emplace_back(turtle);
		return true;
	}
	fprintf(stderr, "Failed to spawn turtle");
	return false;
}

// Creates a new fish and if successfull adds it to the list of fish
bool World::spawn_fish()
{
	Fish fish;
	if (fish.init())
	{
		m_fish.emplace_back(fish);
		return true;
	}
	fprintf(stderr, "Failed to spawn fish");
	return false;
}

// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{
	if (action == GLFW_REPEAT || action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
			m_salmon.should_move(1, true);
			break;
		case GLFW_KEY_RIGHT:
			m_salmon.should_move(2, true);
			break;
		case GLFW_KEY_DOWN:
			m_salmon.should_move(3, true);
			break;
		case GLFW_KEY_UP:
			m_salmon.should_move(4, true);
			break;
		default:
			break;
		}
	}


	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
			m_salmon.should_move(1, false);
			break;
		case GLFW_KEY_RIGHT:
			m_salmon.should_move(2, false);
			break;
		case GLFW_KEY_DOWN:
			m_salmon.should_move(3, false);
			break;
		case GLFW_KEY_UP:
			m_salmon.should_move(4, false);
			break;
		case GLFW_KEY_A:
			std::cout << "Advanced mode activated" << std::endl;
			advanced_mode = true;
			break;
		case GLFW_KEY_B:
			std::cout << "Basic mode activated" << std::endl;
			advanced_mode = false;
			break;
		case GLFW_KEY_SPACE:
			std::cout << "Missile fired" << std::endl;
			if (advanced_mode)
			{
				spawn_fmissile();
			}
			break;
		default:
			break;
		}
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		m_salmon.destroy();
		m_salmon.init();
		m_turtles.clear();
		m_fish.clear();
		m_water.reset_salmon_dead_time();
		m_current_speed = 1.f;
	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) &&  key == GLFW_KEY_COMMA)
		m_current_speed -= 0.1f;
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD)
		m_current_speed += 0.1f;

	m_current_speed = fmax(0.f, m_current_speed);
}

void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// HANDLE SALMON ROTATION HERE
	// xpos and ypos are relative to the top-left of the window, the salmon's
	// default facing direction is (1, 0)
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// 0 is forward (towards right of screen), pi is backwards (towards left)
	// we want to set the range from 0 to 2pi (the unit circle), so we want to calculate the angle in radians
	float rad = static_cast<float>(atan2(ypos - m_salmon.get_position().y, xpos - m_salmon.get_position().x));

	// for some reason it's 1 pi off, so just subtract? haha
	m_salmon.set_rotation(rad);


}

bool World::spawn_fmissile()
{
	Missile missile;
	if (missile.init())
	{
		missile.set_position({ m_salmon.get_position().x,  m_salmon.get_position().y });
		m_missiles.emplace_back(missile);
		return true;
	}
	fprintf(stderr, "Failed to spawn missile");
	return false;
}

// Header
#include "world.hpp"
#include "wall_types.h"
#include "constants.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

// Same as static in c, local to compilation unit
namespace
{
	namespace
	{
		void glfw_err_cb(int error, const char* desc)
		{
			fprintf(stderr, "%d: %s", error, desc);
		}
	}
}

World::World()
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
	m_sam_dead_sound = Mix_LoadWAV(audio_path("sam_dead.wav"));
	m_sam_eat_sound = Mix_LoadWAV(audio_path("sam_eat.wav"));

	if (m_background_music == nullptr || m_sam_dead_sound == nullptr || m_sam_eat_sound == nullptr)
	{
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav"),
			audio_path("sam_dead.wav"),
			audio_path("sam_eat.wav"));
		return false;
	}

	// Playing background music undefinitely
	Mix_PlayMusic(m_background_music, -1);

	fprintf(stderr, "Loaded music\n");

	// Create one long wall
	if (m_sam.init() && m_background.init())
	{
		Wall wall;
		if (wall.init(TALL_WALL))
		{
			// Set at the middle of the top edge of the screen, accounting for half of this wall's height (192)
			wall.set_position({ (screen.x / 2),  192 });
			m_walls.emplace_back(wall);
		}

		Wall wall2;
		if (wall2.init(WIDE_WALL))
		{
			// Set below the previous wall (384 height) accounting for half of this wall's height (19.2)
			wall2.set_position({ (screen.x / 2),  384 + 19.2 });
			m_walls.emplace_back(wall2);
		}

		Wall wall3;
		if (wall3.init(WIDE_WALL))
		{
			// Set left edge of the screen aligned with wall2
			wall3.set_position({ 100,  384 + 19.2 });
			m_walls.emplace_back(wall3);
		}

		Wall wall4;
		if (wall4.init(WIDE_WALL))
		{
			// Set right edge of the screen aligned with wall2
			wall4.set_position({ (screen.x) - 100,  384 + 19.2 });
			m_walls.emplace_back(wall4);
		}

		Wall wall5;
		if (wall5.init(TALL_WALL))
		{
			// Set at bottom edge of the screen
			wall5.set_position({ (screen.x / 2),  (screen.y) - 50 });
			m_walls.emplace_back(wall5);
		}

		Wall wall6;
		if (wall6.init(WIDE_WALL))
		{
			// Set directly above wall 5, shifted to the right by half of its width
			wall6.set_position({ (float) ((screen.x / 2) + (192 - 19.2)),  (screen.y) - 50 - (192) });
			m_walls.emplace_back(wall6);
		}

		Closet closet1;
		if (closet1.init())
		{
			closet1.set_position({ (screen.x / 2) - 250, (screen.y / 2) - 250 });
			m_closets.emplace_back(closet1);
		}

		return true;
	}
	fprintf(stderr, "Failed to spawn wall\n");
	return false;
}

// Releases all the associated resources
void World::destroy()
{
	glDeleteFramebuffers(1, &m_frame_buffer);

	if (m_background_music != nullptr)
		Mix_FreeMusic(m_background_music);
	if (m_sam_dead_sound != nullptr)
		Mix_FreeChunk(m_sam_dead_sound);
	if (m_sam_eat_sound != nullptr)
		Mix_FreeChunk(m_sam_eat_sound);

	Mix_CloseAudio();

	m_sam.destroy();

	glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms)
{
	int w, h;
        glfwGetFramebufferSize(m_window, &w, &h);
	vec2 screen = { (float)w, (float)h };

	// Updating all entities
	m_sam.update(elapsed_ms, m_walls);

	// If sam is dead, restart the game after the fading animation
	if (!m_sam.is_alive() &&
		m_background.get_sam_dead_time() > 5) {
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		m_sam.destroy();
		m_sam.init();
		m_background.reset_sam_dead_time();
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
	glfwSetWindowTitle(m_window, title_ss.str().c_str());

	/////////////////////////////////////
	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	const float clear_color[3] = { 0.1f, 0.1f, 0.1f };
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

	// Drawing walls
	for (auto& wall : m_walls)
			wall.draw(projection_2D);

	// Draw interactable
	for (auto& interactable : m_closets)
		interactable.draw(projection_2D);

	// Drawing Sam
	m_sam.draw(projection_2D);

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

	m_background.draw(projection_2D);

	//////////////////
	// Presenting
	glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over()const
{
	return glfwWindowShouldClose(m_window);
}


// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_sam.direction *= LEFT;
			break;
		case GLFW_KEY_D:
			m_sam.direction *= RIGHT;
			break;
		case GLFW_KEY_S:
			m_sam.direction *= DOWN;
			break;
		case GLFW_KEY_W:
			m_sam.direction *= UP;
			break;
		default:
			break;
		}
	}


	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_sam.direction /= LEFT;
			break;
		case GLFW_KEY_D:
			m_sam.direction /= RIGHT;
			break;
		case GLFW_KEY_S:
			m_sam.direction /= DOWN;
			break;
		case GLFW_KEY_W:
			m_sam.direction /= UP;
			break;

		case GLFW_KEY_E:
			m_sam.interact_in_front(m_closets);
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
		m_sam.destroy();
		m_sam.init();
		m_background.reset_sam_dead_time();
	}

}

void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
}

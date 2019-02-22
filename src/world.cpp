// Header
#include "world.hpp"
#include "DrawSystem.hpp"
#include "InputSystem.hpp"
#include "CollisionSystem.hpp"
#include "TileConstants.hpp"
#include "UpdateAction.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

DrawSystem* ds;
InputSystem* is;
CollisionSystem* cs;

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
	m_points(0)
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
	m_window = glfwCreateWindow((int)screen.x, (int)screen.y, "Stealthy Sam", nullptr, nullptr);
	if (m_window == nullptr)
		return false;

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1); // vsync

	// Load OpenGL function pointers
	gl3w_init();

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	//glfwSetWindowUserPointer(m_window, this);
	//auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	//auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
	//glfwSetKeyCallback(m_window, key_redirect);
	//glfwSetCursorPosCallback(m_window, cursor_pos_redirect);

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

	Texture turtle;

	std::cout << "Screen.x: " << screen.x << std::endl;
	std::cout << "Screen.y: " << screen.y << std::endl;


	// Textures_path needs to be sent this way (can't seem to make it work inside the function)
	generateEntities(map_path("room_one.json"));

	return m_water.init();
}

// Generate entities from a given path to a JSON map file
void World::generateEntities(std::string room_path)
{
	// Components and Object Manager
	ObjectManager om;
	DrawCmp dc;
	TransformCmp tc;
	InputCmp ic;
	CollisionCmp cc;

	int id = 1;

	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = om.makeEntity("Player", id);
	id++;

	// Read JSON map file
	std::ifstream data(room_path);
	json map = json::parse(data);
	json layers = map["layers"];

	// Go through layers
	for (json::iterator layer_it = layers.begin(); layer_it != layers.end(); ++layer_it)
	{
		json tiles = (*layer_it)["data"];

		float y = TILE_HEIGHT / 2;
		float x = TILE_WIDTH / 2;

		// Go through all tiles in this layer
		for (json::iterator tile_it = tiles.begin(); tile_it != tiles.end(); ++tile_it)
		{
			if (x > SCREEN_WIDTH)
			{
				x = TILE_WIDTH / 2;
				y += TILE_HEIGHT;
			}

			// Read tile value
			int val = (*tile_it).get<int>();

			Entity* entity;

			// Generate main player
			if (val == SAM)
			{
				entity = om.getEntity(SAMS_GUID);

				tc.add(entity, { x, y }, { 3.125f, 2.63f }, 0.0);
				dc.add(entity, sam_default_path("Run_01.png"));
				ic.add(entity);
				cc.add(entity);
				vec2 s_position= tc.getTransform(entity)->m_position;
				//vec2 s_position = {200.f, 200.f};
				m_water.add_position(s_position);
			}
			else if (val == WALL)
			{
				entity = om.makeEntity("Wall", id);
				id++;

				tc.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				dc.add(entity, textures_path("Dungeon/wall_mid.png"));
				cc.add(entity);
			}
			else if (val == CLOSET)
			{
				entity = om.makeEntity("Closet", id);
				id++;

				tc.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				dc.add(entity, textures_path("Dungeon/chest_closed.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_1_TO_2)
			{
				entity = om.makeEntity("DoorRoom1To2", id);
				id++;

				tc.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				dc.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_1)
			{
				entity = om.makeEntity("DoorRoom2To1", id);
				id++;

				tc.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				dc.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_3)
			{
				entity = om.makeEntity("DoorRoom2To3", id);
				id++;

				tc.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				dc.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_3_TO_2)
			{
				entity = om.makeEntity("DoorRoom3To2", id);
				id++;

				tc.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				dc.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == ENEMY)
			{
				entity = om.makeEntity("Enemy", id);
				id++;

				tc.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				dc.add(entity, textures_path("Dungeon/enemy.png"));
				cc.add(entity);
			}

			x += TILE_WIDTH;
		}
	}

	// Proceed to initialize systems
	initializeSystems(om, dc, tc, ic, cc);
}

// Set-up DrawSystem, InputSystem, CollisionSystem
void World::initializeSystems(ObjectManager om, DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc)
{
	ds = new DrawSystem(om, dc, tc);
	is = new InputSystem(om, ic, tc, cc);
	cs = new CollisionSystem(om, cc, tc);

	ds->setup();
	is->setup(m_window);
}

// Clear the systems for reinitialization of entities when rooms switch
void World::wipeSystems()
{
	delete ds;
	delete is;
	delete cs;
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
	glfwDestroyWindow(m_window);
}

// Update our game world
// Systems can return an update action to prompt the world to do something
bool World::update(float elapsed_ms)
{
	is->update(elapsed_ms);
	int updateAction = cs->update(elapsed_ms);
	handleUpdateAction(updateAction);
	vec2 s_position= ds->s_position;
	//vec2 s_position= {200.f,200.f};
  m_water.add_position(s_position);


	return true;
}

// Takes in an UpdateAction, handles room changes, death, etc.
void World::handleUpdateAction(int updateAction)
{
	if (updateAction != NO_CHANGE)
	{
		if (updateAction == CHANGE_ROOM_ONE_TO_TWO)
		{
			wipeSystems();
			generateEntities(map_path("room_one_to_two.json"));
		}
		else if (updateAction == CHANGE_ROOM_TWO_TO_ONE)
		{
			wipeSystems();
			generateEntities(map_path("room_two_to_one.json"));
		}
		else if (updateAction == CHANGE_ROOM_TWO_TO_THREE)
		{
			wipeSystems();
			generateEntities(map_path("room_two_to_three.json"));
		}
		else if (updateAction == CHANGE_ROOM_THREE_TO_TWO)
		{
			wipeSystems();
			generateEntities(map_path("room_three_to_two.json"));
		}
		else if (updateAction == COLLIDE_WITH_ENEMY)
		{
			// TODO: Implement death mechanic
		}
	}
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

	ds->update(projection_2D);

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

// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{
	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
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

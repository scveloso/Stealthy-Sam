// Header
#include "world.hpp"
#include "TileConstants.hpp"
#include "UpdateAction.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

EntityGenerator* entityGenerator;

ObjectManager* objectManager;
DrawSystem* ds;
InputSystem* inputSys;
CollisionSystem* cs;
EnemySystem* es;
MovementSystem* ms;
TextSystem* ts;
LightSystem* ls;

// Game State component
GameStateCmp* gameState;

// Entity* keyE;
// Entity *rRestart;



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
	//auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
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


	// Create initial game state
	gameState = new GameStateCmp();
	gameState->init();

	m_water = new Water();
	m_water->init();

	// Textures_path needs to be sent this way (can't seem to make it work inside the function)
	generateEntities(map_path("room_one.json"));

	return true;
}

// Generate entities from a given path to a JSON map file
void World::generateEntities(std::string room_path)
{

	makeSystems();

	// EntityGenerator create entities and init systems
	entityGenerator->generateEntities(room_path, m_water);
	setupWindow();
}

void World::makeSystems()
{
	objectManager = new ObjectManager();
	ds = new DrawSystem();
	inputSys = new InputSystem();
	cs = new CollisionSystem();
	es = new EnemySystem();
	ms = new MovementSystem();
	ts = new TextSystem();
	ls = new LightSystem();

	entityGenerator = new EntityGenerator(objectManager, cs, ds, es, inputSys, ms, ts, ls, gameState);
}

void World::setupWindow()
{
	inputSys->setup(m_window);

	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	glfwSetKeyCallback(m_window, key_redirect);
}

// Clear objects in map for reinitialization of entities when rooms switch
void World::clearMap()
{
	delete entityGenerator;
	delete objectManager;
	delete ds;
	delete inputSys;
	delete cs;
	delete es;
	delete ms;
	delete ts;
	delete ls;

	m_water->clearLights();
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
	// Update Systems
	es->update(elapsed_ms);
	int updateAction = cs->update(elapsed_ms);
	ms->update(elapsed_ms);
	ts->update();
	ls->update();

	// Handle UpdateAction from Systems
	handleUpdateAction(updateAction);

	return true;
}

// Takes in an UpdateAction, handles room changes, restarts, etc.
void World::handleUpdateAction(int updateAction)
{
	if (updateAction != NO_CHANGE)
	{
		if (updateAction == CHANGE_ROOM_ONE_TO_TWO)
		{
			clearMap();
			generateEntities(map_path("room_one_to_two.json"));
			gameState->current_room = ROOM_TWO_GUID;
		}
		else if (updateAction == CHANGE_ROOM_TWO_TO_ONE)
		{
			clearMap();
			generateEntities(map_path("room_two_to_one.json"));
			gameState->current_room = ROOM_ONE_GUID;
		}
		else if (updateAction == CHANGE_ROOM_TWO_TO_THREE)
		{
			clearMap();
			generateEntities(map_path("room_two_to_three.json"));
			gameState->current_room = ROOM_THREE_GUID;
		}
		else if (updateAction == CHANGE_ROOM_THREE_TO_TWO)
		{
			clearMap();
			generateEntities(map_path("room_three_to_two.json"));
			gameState->current_room = ROOM_TWO_GUID;
		}
		else if (updateAction == RESET_GAME)
		{
			gameState->init();
			clearMap();
			generateEntities(map_path("room_one.json"));
			m_water->restart();
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

	m_water->draw(projection_2D);

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
void World::on_key(GLFWwindow*, int key, int _, int action, int mod)
{
    int resultingAction = inputSys->on_key(m_window, key, _, action, mod);
    handleUpdateAction(resultingAction);
}

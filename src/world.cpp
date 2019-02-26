// Header
#include "world.hpp"
#include "DrawSystem.hpp"
#include "InputSystem.hpp"
#include "CollisionSystem.hpp"
#include "EnemySystem.hpp"
#include "MovementSystem.hpp"
#include "TileConstants.hpp"
#include "UpdateAction.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

ObjectManager* objectManager;
DrawSystem* ds;
InputSystem* inputSys;
CollisionSystem* cs;
EnemySystem* es;
MovementSystem* ms;

// Game State component
GameStateCmp* gameState;

Entity* keyE;



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

	Texture turtle;

	// Create initial game state
	gameState = new GameStateCmp();
	gameState->init();

	// Textures_path needs to be sent this way (can't seem to make it work inside the function)
	generateEntities(map_path("room_one.json"));

	return m_water.init();
}

// Generate entities from a given path to a JSON map file
void World::generateEntities(std::string room_path)
{
	// Components and Object Manager
	objectManager = new ObjectManager();
	DrawCmp drawCmp;
	TransformCmp transformCmp;
	InputCmp inputCmp;
	CollisionCmp cc;
	EnemyCmp ec;

	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = objectManager->makeEntity("Player");

	// Create text boxes if we're in room one:
	if (map_path("room_one.json") == room_path)
	{
		// Text boxes
		Entity* useWASD = objectManager->makeEntity(USE_WASD_TEXT_LABEL);
		drawCmp.add(useWASD, textures_path("text/usewasd.png"));
		inputCmp.add(useWASD);
		transformCmp.add(useWASD, { 300, 150 }, { 0.2, 0.2 }, 0.0);
		//pass coordinates to shader
		vec2 tp = transformCmp.getTransform(useWASD)->m_position;
		m_water.add_text(tp);
		if (useWASD->active){
			m_water.removeText = 0;
		}

		Entity* useEText = objectManager->makeEntity(USE_E_INTERACT_LABEL);
		drawCmp.add(useEText, textures_path("text/etointeract.png"));
		inputCmp.add(useEText);
		transformCmp.add(useEText, { 300, 150 }, { 0.2, 0.2 }, 0.0);
		keyE= useEText;
		// Initially the E text box isn't there until we move
		useEText->active = false;
	}


	// Text box if you're dead
	Entity* rToRestart = objectManager->makeEntity(USE_R_RESTART);
	drawCmp.add(rToRestart, textures_path("text/rtorestart.png"));
	transformCmp.add(rToRestart, { 300, 150 }, { 0.2, 0.2 }, 0.0);
	// Initially the you died textbox isn't there until you're dead
	rToRestart->active = false;


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
				entity = objectManager->getEntity(SAMS_GUID);

				transformCmp.add(entity, { x, y }, { 1.125f, 1.5f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/sam.png"));
				//drawCmp.add(entity, textures_path("sam/16.png"));
				inputCmp.add(entity);
				cc.add(entity);
				vec2 s_position = transformCmp.getTransform(entity)->m_position;
				m_water.add_position(s_position);

			}
			else if (val == WALL)
			{
				entity = objectManager->makeEntity("Wall");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/wall_mid.png"));
				cc.add(entity);
			}
			else if (val == KEY)
            {
                entity = objectManager->makeEntity("Key");

                transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
                drawCmp.add(entity, textures_path("Dungeon/key.png"));
                cc.add(entity);
            }
			else if (val == CLOSET)
			{
				entity = objectManager->makeEntity("Closet");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/chest_closed.png"));
				cc.add(entity);

				// Make interactable areas around the closet
				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x + (TILE_WIDTH / 2), y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x - (TILE_WIDTH / 2), y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x, y + (TILE_HEIGHT / 2) }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x, y - (TILE_HEIGHT / 2) }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_1_TO_2)
			{
				entity = objectManager->makeEntity("DoorRoom1To2");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_1)
			{
				entity = objectManager->makeEntity("DoorRoom2To1");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_3)
			{
				entity = objectManager->makeEntity("DoorRoom2To3");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_3_TO_2)
			{
				entity = objectManager->makeEntity("DoorRoom3To2");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == ENEMY)
			{
				entity = objectManager->makeEntity("Enemy");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/ghost.png"));
				cc.add(entity);
				ec.add(entity, 100, 0);
			}

			x += TILE_WIDTH;
		}
	}

	// Proceed to initialize systems

	initializeSystems(drawCmp, transformCmp, inputCmp, cc, ec, gameState);
}

// Set-up DrawSystem, InputSystem, CollisionSystem
void World::initializeSystems(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec,
							  GameStateCmp* gameStateCmp)
{
	ds = new DrawSystem(*objectManager, dc, tc, gameStateCmp);
	inputSys = new InputSystem(*objectManager, ic, tc, cc, gameStateCmp);
	cs = new CollisionSystem(*objectManager, cc, tc, gameStateCmp);
	es = new EnemySystem(*objectManager, cc, tc, ec);
	ms = new MovementSystem(*objectManager, ic, tc, cc, gameStateCmp);

	ds->setup();
	inputSys->setup(m_window);

	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	glfwSetKeyCallback(m_window, key_redirect);
}

// Clear objects in map for reinitialization of entities when rooms switch
void World::clearMap()
{
	delete ds;
	delete inputSys;
	delete cs;
	delete es;
	delete ms;
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

	// Handle UpdateAction
	handleUpdateAction(updateAction);
	if (inputSys->has_move == 1)
	{
		m_water.removeText= 1;
		m_water.removeKey=0;
	}

	if (inputSys->press_keyE == 1)
	{
		m_water.removeKey= 1;

	}

	if (keyE->active)
	{
		vec2 tpe = ds->EBox;
		m_water.add_key(tpe);
		m_water.removeKey = 0;
	}

	// Update sam position for circle of light
	vec2 s_position= ds->s_position;
    m_water.add_position(s_position);
	// vec2 en_position= ds->en_position;
	// m_water.add_enemy_position(en_position);
	// m_water.enemy_direction= ds->en_direction;

    return true;
}

// Takes in an UpdateAction, handles room changes, death, etc.
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
		else if (updateAction == COLLIDE_WITH_ENEMY)
		{
			gameState->sam_is_alive = false;

			// Trigger the death textbox
			m_water.death=1;
			objectManager->getEntityByLabel(USE_R_RESTART)->active = true;
		}
		else if (updateAction == RESET_GAME)
		{
			gameState->init();
			m_water.removeKey=1;
			clearMap();
			generateEntities(map_path("room_one.json"));
			m_water.death=0;
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
void World::on_key(GLFWwindow*, int key, int _, int action, int mod)
{
    int resultingAction = inputSys->on_key(m_window, key, _, action, mod);
    handleUpdateAction(resultingAction);
}

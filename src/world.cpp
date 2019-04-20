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
MissileSystem* missileSystem;

// Game State component
GameStateCmp* gameState;

btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver * solver;
btDiscreteDynamicsWorld* dynamicsWorld;

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
	glfwWindowHint(GLFW_RESIZABLE, 0);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// separate window size for Mac OS
	// m_window = glfwCreateWindow((int)screen.x / 2, (int)screen.y / 2, "Stealthy Sam", nullptr, nullptr);

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

	m_current_speed = 1.f;

	SoundManager::getInstance().init();

	// Create initial game state
	gameState = new GameStateCmp();
	gameState->init();

	// m_water = new Water();
	// m_water->init();
	m_light= new Light();
	m_light->init();
	m_cone= new EnemyCone();
	m_cone->init();
	m_text= new Text();
	m_text->init();

	// Clear fragment shader arrays
	// m_water->clear_enemy_position();
	// m_water->clearSamLight();
	// m_water->clearTorchPositions();
	m_light->clearSamLight();
	m_light->clearTorchPositions();
	m_cone->clear_enemy_position();


	vec2 a = { 0,0 };
	vec2 b = { 0,10 };
	vec2 c = { 10,10 };
	vec2 d = { 10,0 };
	std::vector<vec2> spline;
	spline.emplace_back(a);
	spline.emplace_back(b);
	spline.emplace_back(c);
	spline.emplace_back(d);

	for (float i = 0; i < (float)spline.size() - 3; i += 0.01f) {

		vec2 ans = crSpline(i, spline, 1);
		//printf("%f x:%f y:%f \n", i, ans.x, ans.y);
	}

	// Loading shaders
	if (!standardEffect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;

	generateEntities();

	return true;
}

// Generate entities based on game state
void World::generateEntities()
{
	std::string room_path = "";
	if (gameState->current_room == ROOM_ONE_GUID && gameState->level_one_key && gameState->level_two_key && gameState->level_three_key)
	{
		room_path = map_path("level_one_with_key.json");
	}
	else if (gameState->current_room == ROOM_ONE_GUID) {
		room_path = map_path("level_one.json");
	}
	else if (gameState->current_room == ROOM_TWO_GUID) {
		room_path = map_path("level_two.json");
	}
	else if (gameState->current_room == ROOM_THREE_GUID) {
		room_path = map_path("level_three.json");
	}
	else if (gameState->current_room == ROOM_FOUR_GUID) {
		room_path = map_path("level_four.json");
		SoundManager::getInstance().playBossMusic();
	}
	else if (gameState->current_room == TUTORIAL_LEVEL_GUID) {
		room_path = map_path("level_tutorial.json");
		SoundManager::getInstance().playBackgroundMusic();
	}

	makeSystems();

	// EntityGenerator create entities and init systems
	// entityGenerator->generateEntities(room_path, m_water);
	entityGenerator->generateEntities(room_path, m_light, m_cone, m_text, standardEffect);
	setupWindow();
}

void World::makeSystems()
{
    // Make the Bullet physics world
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, 0, 0));



	objectManager = new ObjectManager();
	ds = new DrawSystem();
	inputSys = new InputSystem();
	cs = new CollisionSystem();
	es = new EnemySystem();
	ms = new MovementSystem();
	ts = new TextSystem();
	ls = new LightSystem();
	missileSystem = new MissileSystem();

	entityGenerator = new EntityGenerator(objectManager, cs, ds, es, inputSys, ms, ts, ls, gameState, missileSystem, dynamicsWorld);
}


void World::setupWindow()
{
	inputSys->setup(m_window);

	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	auto click_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_click(wnd, _0, _1, _2); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
	glfwSetKeyCallback(m_window, key_redirect);
	glfwSetMouseButtonCallback(m_window, click_redirect);
	glfwSetCursorPosCallback(m_window, cursor_pos_redirect);
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
	delete missileSystem;
}

// Releases all the associated resources
void World::destroy()
{
	glDeleteFramebuffers(1, &m_frame_buffer);

	if (m_background_music != nullptr)
		Mix_FreeMusic(m_background_music);
	if (m_death_sound != nullptr)
		Mix_FreeChunk(m_death_sound);
	if (m_salmon_eat_sound != nullptr)
		Mix_FreeChunk(m_salmon_eat_sound);

	Mix_CloseAudio();
	glfwDestroyWindow(m_window);
}

// Update our game world
// Systems can return an update action to prompt the world to do something
bool World::update(float elapsed_ms)
{
	ts->update(elapsed_ms);

	if (gameState->is_game_paused ||
			gameState->in_main_menu ||
			!gameState->sam_is_alive ||
			gameState->in_victory_screen) {
		return true;
	}

	// Update Systems
	int updateAction = es->update(elapsed_ms);
    handleUpdateAction(updateAction);

	updateAction = cs->update(elapsed_ms);
	handleUpdateAction(updateAction);
	ms->update(elapsed_ms);
	ls->update();

	dynamicsWorld->stepSimulation(elapsed_ms);

	return true;
}

// Takes in an UpdateAction, handles room changes, restarts, etc.
void World::handleUpdateAction(int updateAction)
{
	if (updateAction != NO_CHANGE)
	{
		switch (updateAction)
		{
			case TUTORIAL_LEVEL:
			{
				gameState->init();
				gameState->in_main_menu = false;
				clearMap();
				generateEntities();
				m_light->restart();
				m_text->restart();
				m_cone->clear_enemy_position();
				gameState->previous_room = gameState->current_room;
				gameState->current_room = TUTORIAL_LEVEL_GUID;
				gameState->in_tutorial = true;
				generateEntities();
				Entity* entity = objectManager->getEntityByLabel(T1_WELCOME);
				gameState->tutorial_text = entity;
				entity->active = true;
				SoundManager soundManager = SoundManager::getInstance();
				soundManager.haltMusic();
				soundManager.playBackgroundMusic();
				break;
			}
			case CHANGE_TO_ROOM_ONE_ACTION:
			{
				m_cone->clear_enemy_position();
				clearMap();
				gameState->previous_room = gameState->current_room;
				gameState->current_room = ROOM_ONE_GUID;
				generateEntities();
				break;
			}
			case ROOM_TWO:
			{
				clearMap();
				gameState->previous_room = gameState->current_room;
				gameState->current_room = ROOM_TWO_GUID;
				generateEntities();
				m_cone->clear_enemy_position();
				break;
			}
			case ROOM_THREE:
			{
				clearMap();
				gameState->previous_room = gameState->current_room;
				gameState->current_room = ROOM_THREE_GUID;
				generateEntities();
				m_cone->clear_enemy_position();
				break;
			}
			case ROOM_FOUR:
			{
				clearMap();
				gameState->previous_room = gameState->current_room;
				gameState->current_room = ROOM_FOUR_GUID;
				generateEntities();
                gameState->boss_text_countdown_ms = 10000.f;
				objectManager->getEntityByLabel(BOSS_TEXT)->active = true;
				m_cone->clear_enemy_position();
				break;
			}
			case START_NEW_GAME:
			{
				gameState->init();
				gameState->in_main_menu = false;
				clearMap();
                generateEntities();
                // m_water->restart();
				m_light->restart();
				m_text->restart();
				// m_water->clear_enemy_position();
				m_cone->clear_enemy_position();
                gameState->intro_text_countdown_ms = 20000.f;
                objectManager->getEntityByLabel(INTRO_TEXT)->active = true;

				SoundManager soundManager = SoundManager::getInstance();
				soundManager.haltMusic();
				soundManager.playBackgroundMusic();
				break;
			}
			case LOAD_GAME:
			{
				clearMap();
                if (gameState->in_tutorial)
                {
                    gameState->in_tutorial = false;
                }
				generateEntities();
				m_light->restart();
				m_text->restart();
				// m_water->clear_enemy_position();
				m_cone->clear_enemy_position();
				break;
			}
			case SAM_DEATH:
			{
				SoundManager::getInstance().playDeath();
				SoundManager::getInstance().haltMusic();
				break;
			}
			case GAME_WIN:
			{
				gameState->in_victory_screen = true;
				SoundManager::getInstance().playGameEndSound();
				SoundManager::getInstance().haltMusic();
				break;
			}
			case SHOOT_MISSILE:
			{
					std::pair<std::string, Draw*> missile = missileSystem->spawnMissile();
					ds->initializeItem(objectManager->getEntityByLabel(missile.first), missile.second, standardEffect);

					break;
			}
			default:
			{
			    printf("Update Action %d was not recognized", updateAction);
			}
		}
	}
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void World::draw()
{
	// if (gameState->is_game_paused) {
	// 	return;
	// }

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

	// m_water->draw(projection_2D);
	m_text->draw(projection_2D);
	m_cone->draw(projection_2D);
	m_light->draw(projection_2D);
	// glBindFramebuffer(GL_FRAMEBUFFER, 1);
	// glBindTexture(GL_TEXTURE_2D, 1);
	// m_text->draw(projection_2D);



	ds->updateUI(projection_2D);

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

void World::on_mouse_click(GLFWwindow* window, int button, int action, int mods)
{
    int resultingAction = inputSys->on_click(m_window, button, action, mods);
		handleUpdateAction(resultingAction);
}

void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
    gameState->cursor_pos = { (float) xpos, (float) ypos };
}

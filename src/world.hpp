#pragma once

// internal
#include "common.hpp"
#include "water.hpp"
#include "light.hpp"
#include "EnemyCone.hpp"
#include "text.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include "EntityGenerator.hpp"


// stlib
#include <vector>
#include <random>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>
#include <Components/GameStateCmp.hpp>

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class World
{
public:
	World();
	~World();

	// Creates a window, sets up events and begins the game
	bool init(vec2 screen);

	// Generate entities based on game state
	void generateEntities();

	// Set-up DrawSystem, InputSystem, CollisionSystem
	void initializeSystems(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec,
						   GameStateCmp *gameStateCmp);

	void makeSystems();

	// Clear DrawSystem, InputSystem, CollisionSystem
	void clearMap();

	// Takes in an UpdateAction, handles room changes, death, etc.
	void handleUpdateAction(int action);

	void handleParticle(Entity* entity);

	// Releases all associated resources
	void destroy();

	// Steps the game ahead by ms milliseconds
	bool update(float ms);

	// Renders our scene
	void draw();

	// Should the game be over ?
	bool is_over()const;

	void setupWindow();

	Effect standardEffect;

private:
	// !!! INPUT CALLBACK FUNCTIONS
	void on_key(GLFWwindow*, int key, int, int action, int mod);
	void on_mouse_click(GLFWwindow* window, int button, int action, int mods);
	void on_mouse_move(GLFWwindow* window, double xpos, double ypos);

private:
	// Window handle
	GLFWwindow* m_window;

	// Screen texture
	// The draw loop first renders to this texture, then it is used for the water shader
	GLuint m_frame_buffer;
	Texture m_screen_tex;

	// Water effect
	Water* m_water;
	Light* m_light;
	EnemyCone* m_cone;
	Text* m_text;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int m_points;

	float m_current_speed;

	Mix_Music* m_background_music;
	Mix_Chunk* m_death_sound;
	Mix_Chunk* m_salmon_eat_sound;

	// C++ rng
	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_dist; // default 0..1
};

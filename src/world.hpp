#pragma once

// internal
#include "common.hpp"
#include "sam.hpp"
#include "background.hpp"
#include "missile.hpp"
#include "wall.hpp"
#include "enemy.hpp"
#include "closet.hpp"
#include "room.hpp"
#include "roomone.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class World
{
public:
	World();
	~World();

	// Creates a window, sets up events and begins the game
	bool init(vec2 screen);

	// Releases all associated resources
	void destroy();

	// Steps the game ahead by ms milliseconds
	bool update(float ms);

	// Renders our scene
	void draw();

	// Should the game be over ?
	bool is_over()const;

private:

	bool spawn_enemy(float posx, float posy, float patrol_x, float patrol_y);
	// !!! INPUT CALLBACK FUNCTIONS
	void on_key(GLFWwindow*, int key, int, int action, int mod);
	void on_mouse_move(GLFWwindow* window, double xpos, double ypos);

protected:
	// Window handle
	GLFWwindow* m_window;

	// Screen texture
	// The draw loop first renders to this texture, then it is used for the background shader
	GLuint m_frame_buffer;
	Texture m_screen_tex;

	// Background effect
	Background m_background;

	// Game entities
	Sam m_sam;
	Room *m_room; // the current room

	RoomOne m_roomOne; // the first room
	// TODO: Implement second, third and fourth rooms

	std::vector<Enemy> m_enemies;
	std::vector<Wall> m_walls;
	std::vector<Closet> m_closets;

	vec2 m_screen;

	Mix_Music* m_background_music;
	Mix_Chunk* m_sam_dead_sound;
	Mix_Chunk* m_sam_eat_sound;

	// C++ rng
	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_dist; // default 0..1
};

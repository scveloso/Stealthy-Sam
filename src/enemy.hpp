#pragma once

#include "common.hpp"

// stlib
#include <vector>

class Sam;
class Wall;

class Enemy : public Renderable
{

	static Texture enemy_texture;

public:
	// Creates all the associated render resources and default transform
	bool init();

	// Releases all associated resources
	void destroy();

	// Update enemy's position based on direction
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms, std::vector<Wall> m_walls);

	// Renders enemy
	void draw(const mat3& projection)override;

	// Collision routines sam and wall
	// TODO collide with sam
	bool collides_with_wall(vec2 new_position, const Wall& wall);

	void should_move(int direction, bool should);

	float get_half_width()const;

	float get_half_height()const;

	// Returns the current enemy position
	vec2 get_position()const;

	// Moves the enemy's position by the specified offset
	void move(vec2 off);

	// Set enemy's rotation in radians
	void set_rotation(float radians);

	// True if enemy is alive
	bool is_alive()const;

	// Kills enemy, changing its alive state and triggering on death events
	void kill();


private:
	bool m_is_alive; // True if the enemy is alive
	vec2 m_position; // Window coordinates
	vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
	float m_rotation; // in radians
	size_t m_num_indices; // passed to glDrawElements

	bool should_move_left;
	bool should_move_right;
	bool should_move_up;
	bool should_move_down;
	bool should_be_lit_up;

};

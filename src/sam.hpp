#pragma once

#include "common.hpp"

// stlib
#include <vector>

class Turtle;
class Fish;
class Wall;

class Sam : public Renderable
{

	static Texture sam_texture;

public:
	// Creates all the associated render resources and default transform
	bool init();

	// Releases all associated resources
	void destroy();

	// Update Sam's position based on direction
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms, std::vector<Wall> m_walls);

	// Renders Sam
	void draw(const mat3& projection)override;

	// Collision routines for turtles, fish and wall
	bool collides_with(const Turtle& turtle);
	bool collides_with(const Fish& fish);
	bool new_position_collides_with(vec2 new_position, const Wall& wall);

	// Returns the current Sam position
	vec2 get_position()const;

	// Moves the Sam's position by the specified offset
	void move(vec2 off);

	// Set Sam's rotation in radians
	void set_rotation(float radians);

	// True if Sam is alive
	bool is_alive()const;

	// Kills Sam, changing its alive state and triggering on death events
	void kill();

	// Called when the Sam collides with a fish, starts lighting up the Sam
	void light_up();

	void should_move(int direction, bool should);

	float get_half_width()const;

	float get_half_height()const;

private:
	float m_light_up_countdown_ms; // Used to keep track for how long the Sam should be lit up
	bool m_is_alive; // True if the Sam is alive
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

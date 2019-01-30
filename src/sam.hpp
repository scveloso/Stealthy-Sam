#pragma once

#include "common.hpp"
#include "closet.hpp"

// stlib
#include <vector>

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

	// Collision routines for wall
	bool collides_with_wall(vec2 new_position, const Wall& wall);
	bool is_movement_interrupted(vec2 new_position, std::vector<Wall> m_walls);

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

	void should_move(int direction, bool should);

	float get_half_width()const;

	float get_half_height()const;

	void interact_in_front(std::vector<Closet> closets);

	int direction;

private:
	bool m_is_alive; // True if the Sam is alive
	vec2 m_position; // Window coordinates
	vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
	float m_rotation; // in radians
	size_t m_num_indices; // passed to glDrawElements

	bool should_be_lit_up;

};

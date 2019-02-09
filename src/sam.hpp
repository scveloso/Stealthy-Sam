#pragma once

#include "common.hpp"
#include "closet.hpp"
#include "wall.hpp"
#include "door.hpp"

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
	int update(float ms, std::vector<Wall> m_walls, vec2 screen, std::vector<Door> doors);

	// Renders Sam
	void draw(const mat3& projection)override;

	// Collision routines for wall, screen and doors
	bool collides_with_wall(vec2 new_position, const Wall& wall);
	bool collides_with_screen_edge(vec2 new_position, vec2 screen);
	bool collides_with_door(vec2 new_position, const Door& door);
	bool is_movement_interrupted(vec2 new_position, std::vector<Wall> m_walls, vec2 screen);
	int is_entering_door(vec2 new_position, std::vector<Door> doors, vec2 screen);

	// Returns the current Sam position
	vec2 get_position()const;

	// Set Sam's new position
	void set_position(vec2 new_position);

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
	int direction_facing;

private:
	bool m_is_alive; // True if the Sam is alive
	vec2 m_position; // Window coordinates
	vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
	float m_rotation; // in radians
	size_t m_num_indices; // passed to glDrawElements

	bool m_is_hidden; // True if position off the map and hidden in closet
	vec2 m_previous_location; // Previous location for when hidden in a closet
};

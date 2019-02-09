#pragma once

#include "common.hpp"
#include "update_action.hpp"
#include "door_types.h"

class Door : public Renderable
{
	// Shared between all doors, no need to load one for each instance
	static Texture door_texture;

public:
	// Creates all the associated render resources and default transform
	bool init(int updateAction, int doorType);

	// Releases all the associated resources
	void destroy();

	// Update door
	void update(float ms);

	// Renders the door
	// projection is the 2D orthographic projection matrix
	void draw(const mat3& projection)override;

	// Returns the current fish position
	vec2 get_position()const;

	// Sets the new door position
	void set_position(vec2 position);

	vec2 get_texture_dimensions()const;

	// Returns the door bounding box for collision detection, called by collides_with()
	vec2 get_bounding_box()const;

	// Returns the action to be executed when this door is triggered
	int get_update_action()const;

	float get_left_edge()const;

	float get_right_edge()const;

	float get_top_edge()const;

	float get_bottom_edge()const;

	void set_edges();

private:
	vec2 m_position; // Window coordinates
	vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
	float m_rotation; // in radians
	int m_action; // action returned when this door is triggered
	int m_door_type;

	// Coordinates for left edge, right edge, top edge and bottom edge of the object
	float m_x1;
	float m_x2;
	float m_y1;
	float m_y2;
};

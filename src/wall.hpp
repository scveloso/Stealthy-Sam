#pragma once

#include "common.hpp"

// Walls
class Wall : public Renderable
{
	static Texture wall_texture;

public:
	// Creates all the associated render resources and default transform
	bool init();

	// Releases all the associated resources
	void destroy();

	void update(float ms);

	// Renders the wall
	// projection is the 2D orthographic projection matrix
	void draw(const mat3& projection)override;

	// Returns the current wall position
	vec2 get_position()const;

	// Sets the new wall position
	void set_position(vec2 position);

	// Returns the wall's bounding box for collision detection, called by collides_with()
	vec2 get_bounding_box()const;

	vec2 get_texture_dimensions()const;

	float get_left_edge()const;

	float get_right_edge()const;

	float get_top_edge()const;

	float get_bottom_edge()const;

private:
	vec2 m_position; // Window coordinates
	vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
  float m_rotation; // in radians

	// Coordinates for left edge, right edge, top edge and bottom edge of the object
	float m_x1;
	float m_x2;
	float m_y1;
	float m_y2;
};

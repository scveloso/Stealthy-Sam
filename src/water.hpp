#pragma once

#include "common.hpp"


class Water : public Renderable
{
public:
	// Creates all the associated render resources and default transform
	bool init();

	// Releases all associated resources
	void destroy();

	// Renders the water
	void draw(const mat3& projection);

	// Set dead time
	void set_salmon_dead();
	void reset_salmon_dead_time();
	float get_salmon_dead_time() const;
	//add sam_position to be pass down into shaders
	void add_position(vec2 position);
	void add_text(vec2 position);
	void add_key(vec2 position);
	int removeText;
	int removeKey;
	//int enemy_direction;
//	void add_enemy_position(vec2 position);


private:
	// When salmon is alive, the time is set to -1
	float m_dead_time;
	vec2 sam_position;
	vec2 text_position;
	vec2 key_position;
	//std::vector<vec2> enemy_position;
};

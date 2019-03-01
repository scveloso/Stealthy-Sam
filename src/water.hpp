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

	// Game is restarted, change to default values
	void restart();

	// Set dead time
	void set_salmon_dead();
	void reset_salmon_dead_time();
	float get_salmon_dead_time() const;

	//add position to be pass down into shaders to create circle of light
	void add_position(vec2 position);
	int showWASDText;
	int showEText;
	int showRText;
	int death;
	//int enemy_direction;
	//	void add_enemy_position(vec2 position);


private:
	// When salmon is alive, the time is set to -1
	float m_dead_time;
	vec2 circle_light_position;
	vec2 textWASD_position;
	vec2 textE_position;
	vec2 textR_position;
	//std::vector<vec2> enemy_position;
};

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
	void add_enemy_position(int i, vec2 position);
	// left =1; right =2; up =3; down=4; for direction
	void add_enemy_direction(int i, float direction);
	void addTorchPosition(int i, vec2 position);
	void clearTorchPositions();


	int showRText;
	int death;
	int remove_r;
	GLfloat torch_light[10];

  // Clear enemy_position array
	void clear_enemy_position();

	// Clear Sam's light when his held torch is thrown
	void clearSamLight();

private:
	// When salmon is alive, the time is set to -1
	float m_dead_time;
	vec2 circle_light_position;
	vec2 textR_position;
	GLfloat enemy_position[10];
	GLfloat enemy_direction[10];

};

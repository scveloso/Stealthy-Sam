#pragma once

#include "common.hpp"

class Text : public Renderable
{
public:
	bool init();

	void destroy();

	void draw(const mat3& projection);

	// Game is restarted, change to default values
	void restart();

	int showWASDText;
	int showEText;
	int showRText;
	int death;
	int remove_r;

private:
	// When salmon is alive, the time is set to -1
	vec2 textWASD_position;
	vec2 textE_position;
	vec2 textR_position;

};

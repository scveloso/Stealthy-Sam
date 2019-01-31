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
	void draw(const mat3& projection)override;

	// Set dead time
	void set_sam_dead();
	void reset_sam_dead_time();
	float get_sam_dead_time() const;

private:
	// When sam is alive, the time is set to -1
	float m_dead_time;
};

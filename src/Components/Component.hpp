#pragma once

#include "Entity.hpp"
#include "common.hpp"
#include <vector>


class Component
{
public:
	void add(Entity entity);
};

struct Draw : public Renderable
{
	Texture texture;
};

struct Transform
{
	vec2 m_position;
	vec2 m_scale;
	float m_rotation;
	float direction;
	float width;
	float height;
};

struct Collision
{
	bool top;
	bool bottom;
	bool right;
	bool left;
};

const int UP = 2;
const int DOWN = 3;
const int LEFT = 5;
const int RIGHT = 7;
const int NO_DIRECTION = 1;
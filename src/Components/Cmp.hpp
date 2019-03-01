#pragma once

#include "../Entity.hpp"
#include "../common.hpp"
#include <vector>


class Cmp
{
public:
	void add(Entity *entity);
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
	float width;
	float height;
	bool visible;
};

struct Collision
{
	bool closet;
	bool wall;
};

struct Enemy
{
	int type;
	int patrolX;
	int patrolY;
	vec2 start;
	int action;
	//bool chase;
};

struct Movement
{
	int movementDirection;
	float baseSpeed;
	float currSpeed;
	float acceleration;
};

const int NORMAL = 11;

const int UP = 2;
const int DOWN = 3;
const int LEFT = 5;
const int RIGHT = 7;
const int NO_DIRECTION = 1;

const int SAMS_GUID = 0;

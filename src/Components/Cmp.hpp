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
	Texture up;
	Texture down;
	Texture stepup1;
	Texture stepup2;
	Texture stepdown1;
    Texture stepdown2;
    Texture stepside1;
    Texture stepside2;
};

struct Transform
{
	vec2 m_position;
	vec2 m_scale;
	float m_rotation;
	float width;
	float height;
	bool visible;
	int facingDirection;
};

struct Collision
{
	bool closet;
	bool wall;
	float torch_light_countdown_ms;
};

struct Enemy
{
	int dir;
	float t;
	int type;
	int patrolX;
	int patrolY;
	vec2 start;
	int action;
};

// Will hold physics-based attributes
// of appropriate entities.
struct Movement
{
	int movementDirection;
	float baseSpeed;
	float currSpeed;
	float acceleration;
};

struct Item
{
	bool thrown;
};

const int NORMAL = 11;
const int BOSS_ENEMY_TYPE = 12;

const int UP = 2;
const int DOWN = 3;
const int LEFT = 5;
const int RIGHT = 7;
const int NO_DIRECTION = 1;

const int SAMS_GUID = 0;


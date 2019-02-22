#include "TransformCmp.hpp"

void TransformCmp::add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation) {
	Transform *tb = new Transform();
	tb->m_position = m_position;
	tb->m_scale = m_scale;
	tb->m_rotation = m_rotation;
	tb->direction = NO_DIRECTION;
	tb->width = 0.0f;
	tb->height = 0.0f;
	tb->visible = true;
	transform_map[entity->id] = tb;
	printf("Entity ID in Transform: %d\n", entity->id);
}

std::unordered_map<int, Transform *> TransformCmp::getmap()
{
	return transform_map;
}

Transform* TransformCmp::getTransform(Entity *entity) {
	return transform_map[entity->id];
}

void TransformCmp::setPositionX(Entity *entity, float pos)
{
	transform_map[entity->id]->m_position.x = pos;
}

void TransformCmp::setPositionY(Entity *entity, float pos)
{
	transform_map[entity->id]->m_position.y = pos;
}

void TransformCmp::setPosition(Entity *entity, vec2 pos)
{
	transform_map[entity->id]->m_position = pos;
}

void TransformCmp::setDirection(Entity *entity, int direction)
{
	transform_map[entity->id]->direction *= direction;
}

void TransformCmp::removeDirection(Entity *entity, int direction)
{
	float entityDirection = transform_map[entity->id]->direction;
	if (entityDirection != NO_DIRECTION)
	{
		transform_map[entity->id]->direction /= direction;
	}
}

float TransformCmp::getDirection(Entity *entity)
{
	return transform_map[entity->id]->direction;
}

void TransformCmp::setWidth(Entity *entity, float width)
{
	transform_map[entity->id]->width = width;
}

void TransformCmp::setHeight(Entity *entity, float height)
{
	transform_map[entity->id]->height = height;
}

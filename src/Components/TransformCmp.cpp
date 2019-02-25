#include "TransformCmp.hpp"

void TransformCmp::add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation) {
	Transform *tb = new Transform();
	tb->m_position = m_position;
	tb->m_scale = m_scale;
	tb->m_rotation = m_rotation;
	tb->movementDirection = NO_DIRECTION;
	tb->width = 0.0f;
	tb->height = 0.0f;
	tb->visible = true;
	transform_map[entity->id] = tb;
	//printf("Entity ID in Transform: %d\n", entity->id);
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

void TransformCmp::setMovementDirection(Entity *entity, int movementDirection)
{
	transform_map[entity->id]->movementDirection *= movementDirection;
}

void TransformCmp::removeMovementDirection(Entity *entity, int movementDirection)
{
	int entityDirection = transform_map[entity->id]->movementDirection;
	if (entityDirection % movementDirection == 0)
	{
		transform_map[entity->id]->movementDirection /= movementDirection;
	}
}

int TransformCmp::getMovementDirection(Entity *entity)
{
	return transform_map[entity->id]->movementDirection;
}

void TransformCmp::setWidth(Entity *entity, float width)
{
	transform_map[entity->id]->width = width;
}

void TransformCmp::setHeight(Entity *entity, float height)
{
	transform_map[entity->id]->height = height;
}

bool TransformCmp::isGoingLeft(Entity *entity)
{
	return transform_map[entity->id]->movementDirection % LEFT == 0;
}

bool TransformCmp::isGoingRight(Entity *entity)
{
	return transform_map[entity->id]->movementDirection % RIGHT == 0;
}

bool TransformCmp::isGoingUp(Entity *entity)
{
	return transform_map[entity->id]->movementDirection % UP == 0;
}

bool TransformCmp::isGoingDown(Entity *entity)
{
	return transform_map[entity->id]->movementDirection % DOWN == 0;
}

void TransformCmp::faceLeft(Entity *entity)
{
	vec2 entityScale = transform_map[entity->id]->m_scale;
	if (entityScale.x > 0)
	{
		entityScale.x *= -1;
		transform_map[entity->id]->m_scale = entityScale;
	}
}

void TransformCmp::faceRight(Entity *entity)
{
	vec2 entityScale = transform_map[entity->id]->m_scale;
	if (entityScale.x < 0)
	{
		entityScale.x *= -1;
		transform_map[entity->id]->m_scale = entityScale;
	}
}

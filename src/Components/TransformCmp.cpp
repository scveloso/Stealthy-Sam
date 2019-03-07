#include "TransformCmp.hpp"

void TransformCmp::add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation) {
	Transform *tb = new Transform();
	tb->m_position = m_position;
	tb->m_scale = m_scale;
	tb->m_rotation = m_rotation;
	tb->width = 0.0f;
	tb->height = 0.0f;
	tb->visible = true;
	tb->facingDirection = NO_DIRECTION;
	transform_map[entity->id] = tb;
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

void TransformCmp::setWidth(Entity *entity, float width)
{
	transform_map[entity->id]->width = width;
}

void TransformCmp::setHeight(Entity *entity, float height)
{
	transform_map[entity->id]->height = height;
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

bool TransformCmp::isFacingLeft(Entity* entity)
{
	return transform_map[entity->id]->m_scale.x < 0;
}

bool TransformCmp::isFacingRight(Entity* entity)
{
	return transform_map[entity->id]->m_scale.x > 0;
}

bool TransformCmp::isFacingUp(Entity* entity)
{
	return transform_map[entity->id]->m_scale.y > 0;
}

bool TransformCmp::isFacingDown(Entity* entity)
{
	return transform_map[entity->id]->m_scale.x < 0;
}



void TransformCmp::setRotation(Entity* entity, float rotation)
{
	transform_map[entity->id]->m_rotation = rotation;
}

void TransformCmp::setFacingDirection(Entity *entity, int facingDirection)
{
	transform_map[entity->id]->facingDirection = facingDirection;
}

void TransformCmp::removeFacingDirection(Entity *entity, int facingDirection)
{
	int entityDirection = transform_map[entity->id]->facingDirection;

  // Only try remove that direction if facing in that direction
	if (entityDirection % facingDirection == 0)
	{
		int newDirection = entityDirection / facingDirection;

		// We want one facing direction to remain
		if (newDirection != NO_DIRECTION) {
			transform_map[entity->id]->facingDirection = newDirection;
		}
	}
}

int TransformCmp::getFacingDirection(Entity *entity)
{
	return transform_map[entity->id]->facingDirection;
}

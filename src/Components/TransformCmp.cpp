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
	tb->body = NULL;
	transform_map[entity->id] = tb;
}

void TransformCmp::remove(Entity *entity) {
	delete transform_map[entity->id];
	transform_map.erase(entity->id);
}

TransformCmp::~TransformCmp() {
	//printf("TRANSFORM DESTRUCTOR\n"); //called 6 times intially
	//printf("%d \n", transform_map.size());

	for (int i = 0; i < transform_map.size(); i++) {
		delete transform_map[i];
	}
	transform_map.clear();
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
	return transform_map[entity->id]->facingDirection % LEFT == 0;
}

bool TransformCmp::isFacingRight(Entity* entity)
{
	return transform_map[entity->id]->facingDirection % RIGHT == 0;
}

bool TransformCmp::isFacingUp(Entity* entity)
{
	return transform_map[entity->id]->facingDirection % UP == 0;
}

bool TransformCmp::isFacingDown(Entity* entity)
{
	return transform_map[entity->id]->facingDirection % DOWN == 0;
}

void TransformCmp::setRotation(Entity* entity, float rotation)
{
	transform_map[entity->id]->m_rotation = rotation;
}

void TransformCmp::hardSetFacingDirection(Entity *entity, int facingDirection)
{
	transform_map[entity->id]->facingDirection = facingDirection;
}

void TransformCmp::setFacingDirection(Entity *entity, int facingDirection)
{
	if (transform_map[entity->id]->facingDirection % facingDirection != 0) {
		transform_map[entity->id]->facingDirection *= facingDirection;

		if (transform_map[entity->id]->facingDirection == facingDirection) {
			transform_map[entity->id]->prevFacingDirection = facingDirection;
		}
	}
}

void TransformCmp::removeFacingDirection(Entity *entity, int facingDirection)
{
	if (transform_map[entity->id]->facingDirection % facingDirection == 0) {
		transform_map[entity->id]->facingDirection /= facingDirection;
	}
}

void TransformCmp::resetFacingDirection(Entity *entity)
{
	transform_map[entity->id]->prevFacingDirection = NO_DIRECTION;
	transform_map[entity->id]->facingDirection = NO_DIRECTION;
}

int TransformCmp::getFacingDirection(Entity *entity)
{
	return transform_map[entity->id]->facingDirection;
}

int TransformCmp::getPreviousFacingDirection(Entity *entity)
{
	return transform_map[entity->id]->prevFacingDirection;
}

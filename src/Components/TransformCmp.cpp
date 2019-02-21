#include "TransformCmp.hpp"

void TransformCmp::add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation) {
	Transform *tb = new Transform();
	tb->m_position = m_position;
	tb->m_scale = m_scale;
	tb->m_rotation = m_rotation;
	tb->direction = NO_DIRECTION;
	tb->width = 0.0f;
	tb->height = 0.0f;
	transform_list.emplace_back(entity, tb);
	printf("Entity ID in Transform: %d\n", entity->id);
}

std::vector<std::pair<Entity*, Transform*>> TransformCmp::getmap()
{
	return transform_list;
}

Transform* TransformCmp::getTransform(Entity *entity) {
	return transform_list.at(entity->id).second;
}

void TransformCmp::setPositionX(Entity *entity, float pos)
{
	transform_list.at(entity->id).second->m_position.x = pos;
}

void TransformCmp::setPositionY(Entity *entity, float pos)
{
	transform_list.at(entity->id).second->m_position.y = pos;
}

void TransformCmp::setPosition(Entity *entity, vec2 pos)
{
	transform_list.at(entity->id).second->m_position = pos;
}

void TransformCmp::setDirection(Entity *entity, int direction)
{
	//printf("Outside list %d", direction);
	transform_list.at(entity->id).second->direction = direction;
}

void TransformCmp::setWidth(Entity *entity, float width)
{
	transform_list[entity->id].second->width = width;
}

void TransformCmp::setHeight(Entity *entity, float height)
{
	transform_list[entity->id].second->height = height;
}
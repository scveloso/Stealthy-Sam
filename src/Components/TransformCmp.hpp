#pragma once
#include "Component.hpp"

class TransformCmp : public Cmp
{
public:
	void add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation);
	std::unordered_map<int, Transform *> getmap();
	Transform* getTransform(Entity *entity);

	void setPositionX(Entity *entity, float pos);
	void setPositionY(Entity *entity, float pos);
	void setPosition(Entity *entity, vec2 pos);
	
	void setDirection(Entity *entity, int direction);
	void removeDirection(Entity *entity, int direction);

	void setWidth(Entity *entity, float width);
	void setHeight(Entity *entity, float height);

private:
	std::unordered_map<int, Transform*> transform_map;
};

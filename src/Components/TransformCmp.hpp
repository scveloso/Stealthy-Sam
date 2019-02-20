#pragma once
#include "Component.hpp"

class TransformCmp : public Component
{
public:
	void add(Entity entity, vec2 m_position, vec2 m_scale, float m_rotation);
	std::vector<std::pair<Entity, Transform*>> getmap();
	Transform* getTransform(Entity entity);
	
	void setPositionX(Entity entity, float pos);
	void setPositionY(Entity entity, float pos);
	void setPosition(Entity entity, vec2 pos);
	void setDirection(Entity entity, int direction);

	void setWidth(Entity entity, float width);
	void setHeight(Entity entity, float height);

private:
	std::vector<std::pair<Entity, Transform*>> transform_list;
};
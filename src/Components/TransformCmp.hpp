#pragma once
#include "Cmp.hpp"

class TransformCmp : public Cmp
{
public:
	void add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation);
	std::unordered_map<int, Transform *> getmap();
	Transform* getTransform(Entity *entity);

	void setPositionX(Entity *entity, float pos);
	void setPositionY(Entity *entity, float pos);
	void setPosition(Entity *entity, vec2 pos);

	// Movement Direction
	void setMovementDirection(Entity *entity, int movementDirection);
	int getMovementDirection(Entity *entity);
	void removeMovementDirection(Entity *entity, int movementDirection);

	void setWidth(Entity *entity, float width);
	void setHeight(Entity *entity, float height);

	bool isGoingLeft(Entity *entity);
	bool isGoingRight(Entity *entity);
	bool isGoingUp(Entity *entity);
	bool isGoingDown(Entity *entity);

	void faceLeft(Entity *entity);
	void faceRight(Entity *entity);

private:
	std::unordered_map<int, Transform*> transform_map;
};

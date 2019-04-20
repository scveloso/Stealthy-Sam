#pragma once
#include "Cmp.hpp"

class TransformCmp : public Cmp
{
public:
	void add(Entity *entity, vec2 m_position, vec2 m_scale, float m_rotation);
	void remove(Entity *entity);
	std::unordered_map<int, Transform *> getmap();
	Transform* getTransform(Entity *entity);

	void setPositionX(Entity *entity, float pos);
	void setPositionY(Entity *entity, float pos);
	void setPosition(Entity *entity, vec2 pos);

	void setWidth(Entity *entity, float width);
	void setHeight(Entity *entity, float height);

	void faceLeft(Entity *entity);
	void faceRight(Entity *entity);

	bool isFacingLeft(Entity* entity);
	bool isFacingRight(Entity* entity);

	bool isFacingUp(Entity* entity);
	bool isFacingDown(Entity* entity);

	void setRotation(Entity* entity, float rotation);

	void hardSetFacingDirection(Entity *entity, int facingDirection);
	void setFacingDirection(Entity *entity, int facingDirection);
	void removeFacingDirection(Entity *entity, int facingDirection);
	int getFacingDirection(Entity *entity);
	int getPreviousFacingDirection(Entity *entity);
	void resetFacingDirection(Entity *entity);

	~TransformCmp();

private:
	std::unordered_map<int, Transform*> transform_map;
};

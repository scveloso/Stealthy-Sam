#pragma once
#include "Cmp.hpp"

class MovementCmp : public Cmp
{
public:
	void add(Entity* entity, float speed, float acceleration);
	void remove(Entity* entity);
	std::unordered_map<int, Movement *> getmap();

	Movement* getMovement(Entity* entity);
	void setMovementDirection(Entity *entity, int movementDirection);
	void setVecDirection(Entity *entity, vec2 vecDirection);
	void removeMovementDirection(Entity *entity, int movementDirection);
	int getMovementDirection(Entity *entity);
	vec2 getVecDirection(Entity *entity);
	void resetMovementDirection(Entity *entity);

	bool isGoingLeft(Entity *entity);
	bool isGoingRight(Entity *entity);
	bool isGoingUp(Entity *entity);
	bool isGoingDown(Entity *entity);


	float getBaseSpeed(Entity* entity);
	float getStep(Entity* entity, float elapsed_ms);
	void setCurrSpeed(Entity* entity, float currSpeed);

	~MovementCmp();
private:
  std::unordered_map<int, Movement*> movement_map;
};

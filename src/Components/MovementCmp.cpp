#include "MovementCmp.hpp"
#include <string>
#include <iostream>

void MovementCmp::add(Entity *entity, float speed, float acceleration) {
  Movement* mv = new Movement();
  mv->movementDirection = NO_DIRECTION;
  mv->baseSpeed = speed;
  mv->currSpeed = 0;
  mv->acceleration = acceleration;

	movement_map[entity->id] = mv;
}

void MovementCmp::remove(Entity *entity) {
    delete movement_map[entity->id];
    movement_map.erase(entity->id);
}

MovementCmp::~MovementCmp() {
	//printf("MOVEMENTCMP DESTRUCTOR\n");
	for (auto& it : movement_map) {
		delete it.second;
	}
	movement_map.clear();
}

std::unordered_map<int, Movement *> MovementCmp::getmap()
{
	return movement_map;
}

Movement* MovementCmp::getMovement(Entity* entity)
{
  return movement_map[entity->id];
}

float MovementCmp::getBaseSpeed(Entity* entity)
{
  return movement_map[entity->id]->baseSpeed;
}

// Time stepping based physical animation
float MovementCmp::getStep(Entity* entity, float elapsed_ms)
{
  Movement* movement = movement_map[entity->id];

  // If entity uses linear movement
  if (movement->acceleration == 0)
  {
    return movement->baseSpeed * (elapsed_ms / 1000);
  }
  else
  {
    float newSpeed = movement->currSpeed + (movement->acceleration * elapsed_ms);
    movement->currSpeed = newSpeed;
    return newSpeed * (elapsed_ms / 1000);
  }
}

void MovementCmp::setCurrSpeed(Entity* entity, float currSpeed)
{
  Movement* movement = movement_map[entity->id];
  movement->currSpeed = currSpeed;
}

void MovementCmp::setMovementDirection(Entity *entity, int movementDirection)
{
  int entityDirection = movement_map[entity->id]->movementDirection;
  // Only set to that direction if not yet going in that direction
	if (entityDirection % movementDirection != 0)
	{
    movement_map[entity->id]->movementDirection *= movementDirection;
  }
}

void MovementCmp::setVecDirection(Entity *entity, vec2 vecDir)
{
  movement_map[entity->id]->vecDirection = vecDir;
}

void MovementCmp::removeMovementDirection(Entity *entity, int movementDirection)
{
	int entityDirection = movement_map[entity->id]->movementDirection;
  // Only remove that direction if going in that direction
	if (entityDirection % movementDirection == 0)
	{
		movement_map[entity->id]->movementDirection /= movementDirection;
	}
}

int MovementCmp::getMovementDirection(Entity *entity)
{
	return movement_map[entity->id]->movementDirection;
}

vec2 MovementCmp::getVecDirection(Entity *entity)
{
	return movement_map[entity->id]->vecDirection;
}

void MovementCmp::resetMovementDirection(Entity *entity)
{
  movement_map[entity->id]->movementDirection = NO_DIRECTION;
}

bool MovementCmp::isGoingLeft(Entity *entity)
{
	return movement_map[entity->id]->movementDirection % LEFT == 0;
}

bool MovementCmp::isGoingRight(Entity *entity)
{
	return movement_map[entity->id]->movementDirection % RIGHT == 0;
}

bool MovementCmp::isGoingUp(Entity *entity)
{
	return movement_map[entity->id]->movementDirection % UP == 0;
}

bool MovementCmp::isGoingDown(Entity *entity)
{
	return movement_map[entity->id]->movementDirection % DOWN == 0;
}

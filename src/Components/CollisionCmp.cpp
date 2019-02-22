#include "CollisionCmp.hpp"

void CollisionCmp::add(Entity *entity) {
	Collision *collision = new Collision();
	collision->closet = false;
	collision_map[entity->id] = collision;
}

std::unordered_map<int, Collision *> CollisionCmp::getmap()
{
	return collision_map;
}
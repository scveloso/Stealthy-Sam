#include "CollisionCmp.hpp"

void CollisionCmp::add(Entity *entity) {
	Collision *collision = new Collision();
	collision->bottom	= false;
	collision->left		= false;
	collision->right	= false;
	collision->top		= false;
	collision_map[entity->id] = collision;
}

std::unordered_map<int, Collision *> CollisionCmp::getmap()
{
	return collision_map;
}
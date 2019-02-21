#include "CollisionCmp.hpp"

void CollisionCmp::add(Entity *entity) {

	Collision *collision = new Collision();
	collision->bottom	= false;
	collision->left		= false;
	collision->right	= false;
	collision->top		= false;
	collision_list.emplace_back(entity, collision);
}

std::vector<std::pair<Entity*, Collision*>> CollisionCmp::getmap()
{
	return collision_list;
}
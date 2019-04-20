#include "CollisionCmp.hpp"

void CollisionCmp::add(Entity *entity) {
	Collision *collision = new Collision();
	collision->closet = false;
	collision->wall = false;
	collision->torch_light_countdown_ms = -1.f;
	collision_map[entity->id] = collision;
}

CollisionCmp::~CollisionCmp() {
	//printf("COLLISIONCMP DESTRUCTOR\n");
	//printf("%d SIZE \n", collision_map.size());
	for (auto& it : collision_map) {
		delete it.second;
	}
	collision_map.clear();
}

std::unordered_map<int, Collision *> CollisionCmp::getmap()
{
	return collision_map;
}
Collision* CollisionCmp::getCollision(Entity *entity) {
	return collision_map[entity->id];
}

void CollisionCmp::remove(Entity *entity) {
    collision_map.erase(entity->id);

}

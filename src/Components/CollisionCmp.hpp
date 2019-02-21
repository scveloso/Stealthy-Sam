#pragma once
#include "Component.hpp"

class CollisionCmp : public Cmp
{
public:
	void add(Entity *entity);
	std::vector<std::pair<Entity*, Collision*>> getmap();
private:
	std::vector<std::pair<Entity*, Collision*>> collision_list;
};
#pragma once
#include "Component.hpp"

class CollisionCmp : public Component
{
public:
	void add(Entity entity);
	std::vector<std::pair<Entity, Collision*>> getmap();
private:
	std::vector<std::pair<Entity, Collision*>> collision_list;
};
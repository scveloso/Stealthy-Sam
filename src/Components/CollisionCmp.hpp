#pragma once
#include "Cmp.hpp"

class CollisionCmp : public Cmp
{
public:
	void add(Entity *entity);
	std::unordered_map<int, Collision*> getmap();
private:
	std::unordered_map<int, Collision*> collision_map;
};

#pragma once
#include "Component.hpp"

class EnemyCmp : public Cmp
{
public:
	void add(Entity* entity, int patrolX);
	std::unordered_map<int, Enemy *> getmap();
private:
	std::unordered_map<int, Enemy *> enemy_map;
};
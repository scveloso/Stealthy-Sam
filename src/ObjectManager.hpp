#pragma once
#include "Entity.hpp"
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	Entity* makeEntity(std::string label, int gameState);
	Entity* getEntity(int id);

private:
	std::vector<Entity*> entities;
	int num_of_entities;
};

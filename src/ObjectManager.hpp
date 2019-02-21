#pragma once
#include "Entity.hpp"
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	Entity* makeEntity(char* label, int gameState);
	std::vector<Entity*> getEntities();

private:
	std::vector<Entity*> entities;
	int num_of_entities;
};

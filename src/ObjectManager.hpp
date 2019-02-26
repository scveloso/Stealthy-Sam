#pragma once
#include "Entity.hpp"
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	Entity* makeEntity(std::string label);
	Entity* getEntity(int id);
	Entity* getEntityByLabel(std::string label);
private:
	std::vector<Entity*> entities;
	int num_of_entities;
};

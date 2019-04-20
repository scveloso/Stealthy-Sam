#pragma once
#include "Entity.hpp"
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	Entity* makeEntity(std::string label);
	Entity* getEntity(int id);
	Entity* getEntityByLabel(std::string label);
	std::vector<Entity*> getEntitiesByLabel(std::string label);
	void removeEntity(Entity* entity);
private:
	std::vector<Entity*> entities;
	int num_of_entities;
};

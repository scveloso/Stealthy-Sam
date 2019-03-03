#include <iostream>
#include "ObjectManager.hpp"

// Class to create and store entities for a given room
ObjectManager::ObjectManager()
{
	num_of_entities = 0;
}

Entity* ObjectManager::makeEntity(std::string label) {
	Entity *entity = new Entity();
	entity->id = num_of_entities;
	entity->label = label;
  entity->active = true;
	entities.emplace_back(entity);
	num_of_entities++;
	return entity;
}

Entity* ObjectManager::getEntity(int id)
{
	return entities[id];
}

Entity* ObjectManager::getEntityByLabel(std::string label)
{
	for (auto& x : entities)
	{
		if (x->label == label)
		{
			return x;
		}
	}
	return nullptr;
}

std::vector<Entity*> ObjectManager::getEntitiesByLabel(std::string label)
{
	std::vector<Entity*> labelEntities;
	for (auto& x : entities)
	{
		if (x->label == label)
		{
			labelEntities.emplace_back(x);
		}
	}
	return labelEntities;
}

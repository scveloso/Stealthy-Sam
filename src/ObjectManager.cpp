#include "ObjectManager.hpp"

ObjectManager::ObjectManager()
{
	num_of_entities = 0;
}

Entity* ObjectManager::makeEntity(std::string label, int gameState) {
	Entity *entity = new Entity();
	entity->id = num_of_entities;
	entity->label = label;
	entity->gameState = gameState;
	entities.emplace_back(entity);
	num_of_entities++;
	return entity;
}

Entity* ObjectManager::getEntity(int id)
{
	return entities[id];
}

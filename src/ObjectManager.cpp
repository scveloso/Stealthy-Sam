#include "ObjectManager.hpp"

ObjectManager::ObjectManager()
{

}

Entity ObjectManager::makeEntity(int id) {
	//Manually making ids for now
	Entity entity;
	entity.id = id;
	entities.emplace_back(entity);
	return entity;
}

std::vector<Entity> ObjectManager::getEntities()
{
	return entities;
}
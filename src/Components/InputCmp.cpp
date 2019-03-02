#include "InputCmp.hpp"

void InputCmp::add(Entity *entity) {
	input_map[entity->id] = entity;
}

std::unordered_map<int, Entity *> InputCmp::getmap()
{
	return input_map;
}
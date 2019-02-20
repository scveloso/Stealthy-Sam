#include "InputCmp.hpp"

void InputCmp::add(Entity entity) {
	input_list.emplace_back(entity);
}

std::vector<Entity> InputCmp::getmap()
{
	return input_list;
}
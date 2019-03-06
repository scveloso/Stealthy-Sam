#include "ItemCmp.hpp"
#include <string>
#include <iostream>

void ItemCmp::add(Entity *entity) {
  Item* item = new Item();
  item->thrown = false;

	item_map[entity->id] = item;
}

std::unordered_map<int, Item *> ItemCmp::getmap()
{
	return item_map;
}

Item* ItemCmp::getItem(Entity* entity)
{
  return item_map[entity->id];
}

// TODO: Handle keys and other items later
void ItemCmp::pickUpItem(Entity* entity)
{
  Item* item = item_map[entity->id];
  item->thrown = false;
}

void ItemCmp::throwItem(Entity* entity)
{
  Item* item = item_map[entity->id];
  item->thrown = true;
}

int ItemCmp::getThrownTorchId()
{
  for (auto& it : item_map)
	{
    Item* item = it.second;
		if (item->thrown == true)
		{
			return it.first;
		}
	}
	return -1;
}

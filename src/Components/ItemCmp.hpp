#pragma once
#include "Cmp.hpp"

class ItemCmp : public Cmp
{
public:
	void add(Entity* entity);
	std::unordered_map<int, Item *> getmap();

	Item* getItem(Entity* entity);

	void pickUpItem(Entity* entity);
	void throwItem(Entity* entity);

	int getThrownTorchId();
private:
  std::unordered_map<int, Item*> item_map;
};

#pragma once
#include "common.hpp"
#include "water.hpp"
#include "ObjectManager.hpp"
#include "Components/Cmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "Entity.hpp"
#include <vector>
#include <string>

class EntityGenerator
{
public:
	//EntityGenerator(vec2 sam_position);
	void generateEntities(ObjectManager* objectManager, DrawCmp drawCmp, TransformCmp transformCmp,
    InputCmp inputCmp, CollisionCmp cc, EnemyCmp ec, std::string room_path, Water m_water);
private:
	//vec2 m_sam_position;
};

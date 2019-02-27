#pragma once
#include "common.hpp"
#include "Components/Cmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/GameStateCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "water.hpp"
#include "ObjectManager.hpp"
#include "DrawSystem.hpp"
#include "InputSystem.hpp"
#include "CollisionSystem.hpp"
#include "EnemySystem.hpp"
#include "MovementSystem.hpp"
#include "TextSystem.hpp"
#include "Entity.hpp"
#include "water.hpp"

#include <vector>
#include <string>

class EntityGenerator
{
public:
	EntityGenerator(ObjectManager* objectManager, CollisionSystem* collisionSystem, DrawSystem* drawSystem,
		EnemySystem* enemySystem, InputSystem* inputSystem, MovementSystem* movementSystem, TextSystem* textSystem, GameStateCmp* gameState);
	void generateEntities(std::string room_path, Water* water);
	void initializeSystems(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec, Water* water);

private:
	ObjectManager* objectManager;
	CollisionSystem* collisionSystem;
	DrawSystem* drawSystem;
	EnemySystem* enemySystem;
	InputSystem* inputSystem;
	MovementSystem* movementSystem;
	TextSystem* textSystem;
	GameStateCmp* gameState;
};

#pragma once
#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include <string>
#include <iostream>

class DrawSystem
{
public:
	void init(ObjectManager* om, DrawCmp* dc, TransformCmp* tc, MovementCmp* mc, EnemyCmp ec, GameStateCmp* gameStateCmp);
    bool initializeItem(Entity* entity, Draw* draw, Effect effect);
	void update(const mat3 projection);
	void updateUI(const mat3 projection);
	bool setup(Effect effect);
	void playStep();

	vec2 s_position;
	vec2 EBox;
	vec2 rRestart;
	// vec2 en_position;
	// int en_direction;
private:
	DrawCmp* drawComponent;
	EnemyCmp enemyComponent;
	TransformCmp* transformComponent;
	MovementCmp* movementComponent;
	ObjectManager* objectManager;
	GameStateCmp* gameState;

	int stepTimer;
	bool curStep;
};

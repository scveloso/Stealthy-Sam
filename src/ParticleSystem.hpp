#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "Components/ParticleCmp.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include "DrawSystem.hpp"

#include <stdio.h>
#include <sstream>

class ParticleSystem
{
public:
	void init(ObjectManager* om, DrawCmp* dc, TransformCmp* tc, MovementCmp* mc, GameStateCmp* gameStateCmp ,ParticleCmp* particleComponent);
	Entity* update();
	void spawnSmoke(Entity* entity);

	void clearDeadParticles();


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
	ParticleCmp* particleComponent;
	ObjectManager* objectManager;
	GameStateCmp* gameState;
	int numSmoke;
};

const std::string SMOKE_LABEL_PREFIX = "Smoke_";

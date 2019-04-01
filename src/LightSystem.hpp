#include "Components/GameStateCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "water.hpp"
#include "common.hpp"
#include "light.hpp"
#include "text.hpp"
#include "EnemyCone.hpp"

class LightSystem
{
public:
	void init(ObjectManager* om, GameStateCmp* gameStateCmp, TransformCmp* tc, Light* light, EnemyCone* enemy);
	void update();

private:
	ObjectManager* objectManager;
	GameStateCmp* gameState;
	TransformCmp* transformCmp;
  // Water* water;
	Light* light;
	EnemyCone* enemy;
};

#include "Components/GameStateCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "water.hpp"
#include "common.hpp"

class LightSystem
{
public:
	void init(ObjectManager om, GameStateCmp* gameStateCmp, TransformCmp tc, Water* water);
	void update();

private:
	ObjectManager objectManager;
	GameStateCmp* gameState;
	TransformCmp transformCmp;
  Water* water;
};

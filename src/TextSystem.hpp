#include "Components/GameStateCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "water.hpp"
#include "common.hpp"

class TextSystem
{
public:
	void init(ObjectManager om, GameStateCmp* gameStateCmp, Water* water);
	void update();

private:
	ObjectManager objectManager;
	GameStateCmp* gameState;
  Water* water;
};

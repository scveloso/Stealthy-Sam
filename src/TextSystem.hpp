#include "Components/GameStateCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "text.hpp"
#include "EnemyCone.hpp"
#include "light.hpp"
#include "common.hpp"

class TextSystem
{
public:
	void init(ObjectManager* om, GameStateCmp* gameStateCmp, Text* text, Light* light, EnemyCone* enemy);
	void update();
	void enableEText();
	void disableEText();
	void disableWASDText();
	void enableTutorialScreen();
	void disableTutorialScreen();

private:
	ObjectManager* objectManager;
	GameStateCmp* gameState;
  Text* text;
	Light* light;
	EnemyCone* enemy;
};

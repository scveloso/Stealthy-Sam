#include "Components/GameStateCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include "text.hpp"
#include "EnemyCone.hpp"
#include "light.hpp"
#include "common.hpp"

class TextSystem
{
public:
	void init(ObjectManager* om, GameStateCmp* gameStateCmp, Text* text, Light* light, EnemyCone* enemy);
	void update(float elapsed_ms);
	void handleGameSavedAlert(float elapsed_ms);
	void handleGameLoadedAlert(float elapsed_ms);
	void handleGameDeathAlert();
	void handleVictoryScreen();

private:
	ObjectManager* objectManager;
	GameStateCmp* gameState;
  Text* text;
	Light* light;
	EnemyCone* enemy;
	float gameLoadedMs;
	float gameSavedMs;

    void handleTutorialLighting() const;
};

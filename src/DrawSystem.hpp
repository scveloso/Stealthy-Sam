#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "ObjectManager.hpp"

class DrawSystem
{
public:
	void init(ObjectManager om, DrawCmp dc, TransformCmp tc, MovementCmp mc, GameStateCmp* gameStateCmp);
	void update(const mat3 projection);
	void updateUI(const mat3 projection);
	bool setup();
	void del();
	vec2 s_position;
	vec2 EBox;
	vec2 rRestart;
	// vec2 en_position;
	// int en_direction;
private:
	DrawCmp drawComponent;
	TransformCmp transformComponent;
	MovementCmp movementComponent;
	ObjectManager objectManager;
	GameStateCmp* gameState;

	int stepTimer;
	bool curStep;
};

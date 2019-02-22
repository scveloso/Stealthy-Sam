#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "ObjectManager.hpp"

class DrawSystem
{
public:
	DrawSystem(ObjectManager om, DrawCmp dc, TransformCmp tc, GameStateCmp* gameStateCmp);
	void update(const mat3 projection);
	bool setup();
	vec2 s_position;
private:
	DrawCmp drawComponent;
	TransformCmp transformComponent;
	ObjectManager objectManager;
	GameStateCmp* gameState;
};

#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "ObjectManager.hpp"

class DrawSystem
{
public:
	void init(ObjectManager om, DrawCmp dc, TransformCmp tc, GameStateCmp* gameStateCmp);
	void update(const mat3 projection);
	bool setup();
	// vec2 en_position;
	// int en_direction;
private:
	DrawCmp drawComponent;
	TransformCmp transformComponent;
	ObjectManager objectManager;
	GameStateCmp* gameState;
};

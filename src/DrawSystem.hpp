#include "Components/DrawCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "ObjectManager.hpp"

class DrawSystem
{
public:
	DrawSystem(ObjectManager om, DrawCmp dc, TransformCmp tc);
	void update(const mat3 projection);
	bool setup();
	vec2 s_position;
	// vec2 moveBox;
	vec2 EBox;
private:
	DrawCmp drawComponent;
	TransformCmp transformComponent;
	ObjectManager objectManager;
};

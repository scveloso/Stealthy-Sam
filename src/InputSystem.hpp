#include "Components/TransformCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "common.hpp"

class InputSystem
{
public:
	InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc);
	void on_key(GLFWwindow*, int key, int, int action, int mod);
	bool setup(GLFWwindow* m_window);
	void update(float elapsed_ms);
	int move;
	int keyE;

private:
	InputCmp inputComponent;
	TransformCmp transformComponent;
	CollisionCmp collisionComponent;
	ObjectManager objectManager;
};

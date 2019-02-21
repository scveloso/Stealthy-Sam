#include "InputSystem.hpp"
#include "Components/Component.hpp"

InputSystem::InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc)
{
	inputComponent = ic;
	transformComponent = tc;
	collisionComponent = cc;
}

bool InputSystem::setup(GLFWwindow* m_window)
{
	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((InputSystem*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	glfwSetKeyCallback(m_window, key_redirect);
	return true;
}

void InputSystem::on_key(GLFWwindow*, int key, int, int action, int mod)
{

	for (auto& it : inputComponent.getmap())
	{
		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_A:
				if (!collisionComponent.getmap().at(it->id).second->left) {
					transformComponent.setDirection(it, LEFT);
				}
				break;
			case GLFW_KEY_D:
				transformComponent.setDirection(it, RIGHT);
				break;
			case GLFW_KEY_S:
				transformComponent.setDirection(it, DOWN);
				break;
			case GLFW_KEY_W:
				transformComponent.setDirection(it, UP);
				break;
			case GLFW_KEY_E:
				transformComponent.setDirection(it, LEFT);
			case GLFW_KEY_P:
				//print out current position
				printf("X:%f , Y:%f \n", transformComponent.getTransform(it)->m_position.x, transformComponent.getTransform(it)->m_position.y);
				break;
			case GLFW_KEY_C:
				//print out collision stuff
				Transform *tr1 = transformComponent.getTransform(it);
				vec2 size1 = { tr1->width*tr1->m_scale.x, tr1->height*tr1->m_scale.y };

				printf("%f, %f, \n", tr1->width, tr1->height);

				printf("SIZEX:%f , SIZEY:%f \n", transformComponent.getTransform(it)->m_position.x + size1.x, transformComponent.getTransform(it)->m_position.y + size1.y); 
				break;
			}

		}

		if (action == GLFW_RELEASE)
		{
			switch (key)
			{
			case GLFW_KEY_A:
			case GLFW_KEY_D:
			case GLFW_KEY_S:
			case GLFW_KEY_W:
				transformComponent.setDirection(it, NO_DIRECTION);
			default:
				break;
			}
		}


	}
}

void InputSystem::update(float elapsed_ms)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (elapsed_ms / 1000);

	for (auto& it : inputComponent.getmap())
	{
		int direction = transformComponent.getTransform(it)->direction;
		vec2 new_position = transformComponent.getTransform(it)->m_position;

		if (direction % LEFT == 0)
		{
			new_position.x = new_position.x - step;
		}

		if (direction % RIGHT == 0)
		{
			new_position.x = new_position.x + step;
		}

		if (direction % DOWN == 0)
		{
			new_position.y = new_position.y + step;
		}

		if (direction % UP == 0)
		{
			new_position.y = new_position.y - step;
		}

		transformComponent.setPosition(it, new_position);
	}
}
#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Component.hpp"
#include <iostream>
#include <string>

InputSystem::InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc)
{
    objectManager = om;
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
    bool didMove = false;
	for (auto& it : inputComponent.getmap())
	{
	    Entity* entity = objectManager.getEntity(it.first);
	    if (entity->label == USE_WASD_TEXT_LABEL || entity->label == USE_E_INTERACT_LABEL)
        {
	        // skip
        } else {
            if (action == GLFW_PRESS)
            {
                switch (key) {
                    case GLFW_KEY_A:
                        if (!collisionComponent.getmap()[it.first]->left) {
                            transformComponent.setDirection(entity, LEFT);
                        std::cout << transformComponent.getDirection(entity) << std::endl;}
                        didMove = true;
                        break;
                    case GLFW_KEY_D:
                        transformComponent.setDirection(entity, RIGHT);std::cout << transformComponent.getDirection(entity) << std::endl;
                        didMove = true;
                        break;
                    case GLFW_KEY_S:
                        transformComponent.setDirection(entity, DOWN);std::cout << transformComponent.getDirection(entity) << std::endl;
                        didMove = true;
                        break;
                    case GLFW_KEY_W:
                        transformComponent.setDirection(entity, UP);std::cout << transformComponent.getDirection(entity) << std::endl;
                        didMove = true;
                        break;
                    case GLFW_KEY_E:
                        // TODO: Implement interactables
                        break;
                    case GLFW_KEY_P:
                        //print out current position
                        printf("X:%f , Y:%f \n", transformComponent.getTransform(entity)->m_position.x,
                               transformComponent.getTransform(entity)->m_position.y);
                        break;
                    case GLFW_KEY_C:
                        //print out collision stuff
                        Transform *tr1 = transformComponent.getTransform(entity);
                        vec2 size1 = {tr1->width * tr1->m_scale.x, tr1->height * tr1->m_scale.y};

                        printf("%f, %f, \n", tr1->width, tr1->height);

                        printf("SIZEX:%f , SIZEY:%f \n",
                               transformComponent.getTransform(entity)->m_position.x + size1.x,
                               transformComponent.getTransform(entity)->m_position.y + size1.y);
                        break;
                }

            }

            if (action == GLFW_RELEASE)
            {
                switch (key)
                {
                    case GLFW_KEY_A:
                        transformComponent.removeDirection(entity, LEFT);
                        break;
                    case GLFW_KEY_D:
                        transformComponent.removeDirection(entity, RIGHT);
                        break;
                    case GLFW_KEY_S:transformComponent.removeDirection(entity, DOWN);
                        break;
                    case GLFW_KEY_W:
                        transformComponent.removeDirection(entity, UP);
                        break;
                    default:
                        break;
                }
            }
        }
	}

	if (didMove) {
	    objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL)->active = false;
	}
}

void InputSystem::update(float elapsed_ms)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (elapsed_ms / 1000);

	for (auto& it : inputComponent.getmap())
	{
		Entity* entity = objectManager.getEntity(it.first);

		int direction = transformComponent.getTransform(entity)->direction;

		vec2 new_position = transformComponent.getTransform(entity)->m_position;

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

		transformComponent.setPosition(entity, new_position);
	}
}

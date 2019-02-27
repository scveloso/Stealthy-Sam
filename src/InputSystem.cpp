#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Cmp.hpp"
#include "UpdateAction.hpp"
#include <iostream>
#include <string>

void InputSystem::init(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, GameStateCmp* gameStateCmp)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
    gameState = gameStateCmp;
}

bool InputSystem::setup(GLFWwindow* m_window)
{
	return true;
}

int InputSystem::on_key(GLFWwindow *, int key, int _, int action, int mod)
{
  int returnAction = NO_CHANGE;
    bool didMove = false;
    bool didPressE = false;

	for (auto& it : inputComponent.getmap())
	{
    Entity* entity = objectManager.getEntity(it.first);
	  if (entity->label == USE_WASD_TEXT_LABEL || entity->label == USE_E_INTERACT_LABEL)
    {
      // NO-OP
    }
    else
    {
      if (action == GLFW_PRESS)
      {
        float direction = transformComponent.getMovementDirection(entity);
        switch (key) {
          case GLFW_KEY_A:
            if (!transformComponent.isGoingLeft(entity))
            {
              transformComponent.setMovementDirection(entity, LEFT);
              if (gameState->sam_is_alive) {
                transformComponent.faceLeft(entity);
              }
              didMove = true;
            }
            break;
          case GLFW_KEY_D:
          if (!transformComponent.isGoingRight(entity))
          {
            transformComponent.setMovementDirection(entity, RIGHT);
            if (gameState->sam_is_alive) {
              transformComponent.faceRight(entity);
            }
            didMove = true;
          }
            break;
          case GLFW_KEY_S:
            transformComponent.setMovementDirection(entity, DOWN);
            didMove = true;
            break;
          case GLFW_KEY_W:
            transformComponent.setMovementDirection(entity, UP);
            didMove = true;
            break;
          case GLFW_KEY_E:
            // TODO: Implement interactables
  			if (it.first == SAMS_GUID && collisionComponent.getmap().at(SAMS_GUID)->closet == true)
            {
  			    if (transformComponent.getTransform(entity)->visible == false)
                {
  				    transformComponent.getTransform(entity)->visible = true;
  				}
  				else
          {
  				    transformComponent.getTransform(entity)->visible = false;
  				}
  			}
            didPressE = true;
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
          switch (key) {

                    case GLFW_KEY_A:
                        transformComponent.removeMovementDirection(entity, LEFT);
                        break;
                    case GLFW_KEY_D:
                        transformComponent.removeMovementDirection(entity, RIGHT);
                        break;
                    case GLFW_KEY_S:
                        transformComponent.removeMovementDirection(entity, DOWN);
                        break;
                    case GLFW_KEY_W:
                        transformComponent.removeMovementDirection(entity, UP);
                        break;
					case GLFW_KEY_R:
						returnAction = RESET_GAME;
						break;
                    default:
                        break;
				}
			}
		}
	}


	// Deal with tutorial textboxes only if we're in room one:
	if (gameState->current_room == ROOM_ONE_GUID && objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL) != nullptr) {
		// If we move using WASD, remove the text box showing movement directions
		if (didMove)
			{Entity* wasd_text_ent = objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL);
			has_move = 1;
			// If we've moved and haven't already made this inactive, trigger the second text box (press E) to appear
			if (wasd_text_ent->active) {
				wasd_text_ent->active = false;
				objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = true;
			}
		}

		// Once they press E, we can remove the E text box
		if (didPressE)
			{press_keyE = 1;
			objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = false;
		}
	}

	return returnAction;
}

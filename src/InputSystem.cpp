#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Cmp.hpp"
#include "UpdateAction.hpp"
#include <iostream>
#include <string>
#include "TileConstants.hpp"

void InputSystem::init(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, MovementCmp mc, GameStateCmp* gameStateCmp)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
    movementComponent = mc;
    gameState = gameStateCmp;
}

bool InputSystem::setup(GLFWwindow* m_window)
{
	return true;
}

int InputSystem::on_key(GLFWwindow *, int key, int _, int action, int mod)
{
  int returnAction = NO_CHANGE;

  // Update relevant entities based on input key
	for (auto& it : inputComponent.getmap())
	{
    Entity* entity = objectManager.getEntity(it.first);

    // Handle inputs for Sam
    if (entity->label.compare("Player") == 0)
    {
      if (action == GLFW_PRESS)
      {
        float direction = movementComponent.getMovementDirection(entity);
        switch (key) {
          case GLFW_KEY_A:
            if (!movementComponent.isGoingLeft(entity))
            {
              movementComponent.setMovementDirection(entity, LEFT);
              if (gameState->sam_is_alive) {
                transformComponent.faceLeft(entity);
              }
              gameState->has_moved = true;
            }
            break;
          case GLFW_KEY_D:
            if (!movementComponent.isGoingRight(entity))
            {
              movementComponent.setMovementDirection(entity, RIGHT);
              if (gameState->sam_is_alive) {
                transformComponent.faceRight(entity);
              }
              gameState->has_moved = true;
            }
            break;
          case GLFW_KEY_S:
            movementComponent.setMovementDirection(entity, DOWN);
            gameState->has_moved = true;
            break;
          case GLFW_KEY_W:
            movementComponent.setMovementDirection(entity, UP);
            gameState->has_moved = true;
            break;
          case GLFW_KEY_E:
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
            gameState->has_pressed_E = true;
            break;
          case GLFW_KEY_P:
            // Print out current position
            printf("X:%f , Y:%f \n", transformComponent.getTransform(entity)->m_position.x,
            transformComponent.getTransform(entity)->m_position.y);
            break;
          case GLFW_KEY_C:
          {
            // Print out collision stuff
            Transform *tr1 = transformComponent.getTransform(entity);
            vec2 size1 = {tr1->width * tr1->m_scale.x, tr1->height * tr1->m_scale.y};

            printf("%f, %f, \n", tr1->width, tr1->height);
            printf("SIZEX:%f , SIZEY:%f \n",
                               transformComponent.getTransform(entity)->m_position.x + size1.x,
                               transformComponent.getTransform(entity)->m_position.y + size1.y);
            break;
          }
          case GLFW_KEY_T:
          {
            Entity* heldEntity = gameState->held_entity;

            if (heldEntity && heldEntity->label.compare("Torch") == 0)
            {
              heldEntity->active = true;
              Movement* entityMovement = movementComponent.getMovement(heldEntity);
              movementComponent.resetMovementDirection(heldEntity);

              // Set the throwable's movement-physics attributes to move to the left
              if (transformComponent.isFacingLeft(entity)) {
                vec2 torch_position = { gameState->sam_position.x - (TILE_WIDTH), gameState->sam_position.y };
                transformComponent.setPosition(heldEntity, torch_position);
                movementComponent.setCurrSpeed(heldEntity, entityMovement->baseSpeed);
                movementComponent.setMovementDirection(heldEntity, LEFT);
              }

              // Set the throwable's movement-physics attributes to move to the right
              if (transformComponent.isFacingRight(entity)) {
                vec2 torch_position = { gameState->sam_position.x + (TILE_WIDTH), gameState->sam_position.y };
                transformComponent.setPosition(heldEntity, torch_position);
                movementComponent.setCurrSpeed(heldEntity, entityMovement->baseSpeed);
                movementComponent.setMovementDirection(heldEntity, RIGHT);
              }

              gameState->held_item = -1;
              gameState->held_entity = NULL;
            }

            break;
          }
        }
      }
      if (action == GLFW_RELEASE)
      {
        switch (key) {
          case GLFW_KEY_A:
            movementComponent.removeMovementDirection(entity, LEFT);
            break;
          case GLFW_KEY_D:
            movementComponent.removeMovementDirection(entity, RIGHT);
            break;
          case GLFW_KEY_S:
            movementComponent.removeMovementDirection(entity, DOWN);
            break;
          case GLFW_KEY_W:
            movementComponent.removeMovementDirection(entity, UP);
            break;
          default:
            break;
        }
      }
    }
	}

  if (action == GLFW_RELEASE)
  {
    switch (key) {
      case GLFW_KEY_R:
        returnAction = RESET_GAME;
        break;
      default:
        break;
    }
  }

	return returnAction;
}

#include "TextSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>
#include <Components/GameStateCmp.hpp>

// System to update alert entities in the game.
//
// Has access to GameStateCmp to know about whether Sam has died, etc.
//
// Has access to alert entities to set them to active/inactive and handle their timeout.
void TextSystem::init(ObjectManager* om, GameStateCmp* gameStateCmp, Text* text, Light* light, EnemyCone* enemy)
{
  objectManager = om;
  gameState = gameStateCmp;
  this->text = text;
  this->light = light;
  this->enemy= enemy;

  gameLoadedMs = 0.0;
  gameSavedMs = 0.0;
}

void TextSystem::update(float elapsed_ms)
{
  handleGameLoadedAlert(elapsed_ms);
  handleGameSavedAlert(elapsed_ms);
  handleGameDeathAlert();
  handleVictoryScreen();
  handleTutorialLighting();
}

void TextSystem::handleGameLoadedAlert(float elapsed_ms) {
  Entity* game_loaded = objectManager->getEntityByLabel(GAME_LOADED_ALERT);
  if (game_loaded->active) {
    objectManager->getEntityByLabel(GAME_SAVED_ALERT)->active = false;
    if (gameLoadedMs <= 0) {
      gameLoadedMs = 2000;
    }

    gameLoadedMs -= elapsed_ms;

    if (gameLoadedMs <= 0) {
      game_loaded->active = false;
    }
  }
}

void TextSystem::handleGameSavedAlert(float elapsed_ms) {
  // Handle timing out the "game saved" alert.
  Entity* game_saved = objectManager->getEntityByLabel(GAME_SAVED_ALERT);
  if (game_saved->active) {
    if (gameSavedMs <= 0) {
      gameSavedMs = 2000;
    }

    gameSavedMs -= elapsed_ms;

    if (gameSavedMs <= 0) {
      game_saved->active = false;
    }
  }
}

void TextSystem::handleGameDeathAlert() {
  // If Sam died, enable death text
  if (!gameState->sam_is_alive)
  {
    objectManager->getEntityByLabel(GAME_SAVED_ALERT)->active = false;
    objectManager->getEntityByLabel(GAME_LOADED_ALERT)->active = false;
    light->death=1;
    enemy->death=1;
    objectManager->getEntityByLabel(GAME_DEATH_ALERT)->active = true;
  }
}

void TextSystem::handleTutorialLighting() const {
  if (gameState->in_tutorial)
  {
    light->tutorial=1;
  }
}


void TextSystem::handleVictoryScreen() {
  // If Sam died, enable death text
  if (gameState->in_victory_screen)
  {
    SoundManager::getInstance().haltMusic();
    objectManager->getEntityByLabel(VICTORY_SCREEN)->active = true;
  }
}

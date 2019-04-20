#include "GameStateCmp.hpp"

#include <iostream>
#include <string>

using json = nlohmann::json;

// Start a new game
void GameStateCmp::init() {
    sam_is_alive = true;
    level_one_key = false;
    level_two_key = false;
    level_three_key = false;
    previous_room = "";
    current_room = ROOM_ONE_GUID;

    // Tutorial status
    has_moved = false;
    has_pressed_H = false;
    boss_door_text_countdown_ms = -1.f;
    boss_text_countdown_ms = -1.f;
    intro_text_countdown_ms = -1.f;

    // New game is unpaused, not loading and not hidden
    is_game_paused = false;
    is_game_loading = false;
    hidden = false;

    // Held entity
    held_item = -1;
    held_entity = NULL;

    // text entities
    tutorial_text = NULL;

    num_lit_cauldrons = 0;
    in_main_menu = true;
    in_victory_screen = false;
    in_tutorial = false;
}

// Save game to file, provided Sam is alive and the game is paused
void GameStateCmp::saveGame() {
  if (sam_is_alive && is_game_paused) {
    json savefile;
    savefile["level_one_key"] = level_one_key;
    savefile["level_two_key"] = level_two_key;
    savefile["level_three_key"] = level_three_key;
    savefile["previous_room"] = previous_room;
    savefile["current_room"] = current_room;
    savefile["sam_position_x"] = sam_position.x;
    savefile["sam_position_y"] = sam_position.y;
    savefile["hidden"] = hidden;
    savefile["has_moved"] = has_moved;
    savefile["has_pressed_H"] = has_pressed_H;
    savefile["held_item"] = held_item;

    std::ofstream save_file(saves_path("save_file.json"));
    save_file << savefile;

    std::cout << "Saved game." << std::endl;
  }
}

// Returns true if game successfully loaded from save, false otherwise
bool GameStateCmp::loadGame() {
  if (is_game_paused || in_main_menu) {
    // Read JSON save file
  	std::ifstream data(saves_path("save_file.json"));
    if (data.fail()) {
      std::cout << "Couldn't read save file. " << std::endl;
      return false;
    }
  	json savefile = json::parse(data);

    // Populate game state based on save file

    // Load Sam progress
    level_one_key = savefile["level_one_key"];
  	level_two_key = savefile["level_two_key"];
    level_three_key = savefile["level_three_key"];
    previous_room = savefile["previous_room"];
    current_room = savefile["current_room"];
    sam_is_alive = true;

    // Load Sam position
    float sam_position_x = savefile["sam_position_x"];
    float sam_position_y = savefile["sam_position_y"];
    sam_position = { sam_position_x, sam_position_y };

    hidden = savefile["hidden"];

    // Load tutorial attributes
    has_moved = savefile["has_moved"];
    has_pressed_H = savefile["has_pressed_H"];
    held_item = savefile["held_item"];

    // Start game when reloaded
    num_lit_cauldrons = 0;
    is_game_paused = false;
    in_main_menu = false;
    is_game_loading = true;
    in_victory_screen = false;

    std::cout << "Loaded game." << std::endl;

    return true;
  }

  return false;
}

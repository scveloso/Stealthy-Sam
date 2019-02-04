#pragma once

#include "room.hpp"
#include "common.hpp"

class RoomTwo : public Room
{
  public:
    bool init(vec2 screen);

  	// Steps the game ahead by ms milliseconds
  	bool update(float ms, vec2 screen);
};

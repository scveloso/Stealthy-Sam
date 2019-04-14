#pragma once
#include "common.hpp"

class Light : public Renderable
{
public:
  bool init();
  void destroy();
  void draw(const mat3& projection);
  void restart();
  void add_position(vec2 position);
  void addTorchPosition(int i, vec2 position);
  void clearTorchPositions();
  void clearSamLight();
  int death;
  int tutorial;
  GLfloat torch_light[10];

private:
  vec2 circle_light_position;
};

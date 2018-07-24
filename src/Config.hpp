#pragma once
#include <cstdio>
#include "Vector.hpp"

struct GameConfig
{
  float ballSpeed;
  float paddleSpeed;
  Vec2f paddleSize;
  Vec2f ballSize;
};

void initConfig(GameConfig*);

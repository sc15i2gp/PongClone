#pragma once
#include "Platform.hpp"
#include "Vector.hpp"

struct GameState
{
  Vec2f rectPosition;
  Drawable rect;
  uint shader;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState);

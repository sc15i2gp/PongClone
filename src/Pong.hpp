#pragma once
#include "Platform.hpp"
#include "Vector.hpp"

struct GameState
{
  Vec2f paddle1Position;
  Vec2f paddle2Position;
  Vec2f ballPosition;
  Drawable paddle1;
  Drawable paddle2;
  Drawable ball;
  uint shader;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState);

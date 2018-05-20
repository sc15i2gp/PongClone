#pragma once
#include "Platform.hpp"
#include "Vector.hpp"

//Paddle or ball
struct Entity
{
  Vec2f size;
  Vec2f velocity;
  Vec2f position;
};

struct GameState
{
  Entity paddle1Entity;
  Entity paddle2Entity;
  Entity ballEntity;
  Drawable paddle1;
  Drawable paddle2;
  Drawable ball;
  uint shader;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState);

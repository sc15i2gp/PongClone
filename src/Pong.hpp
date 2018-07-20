#pragma once
#include <cstdio>
#include "Platform.hpp"
#include "Entity.hpp"
#include "Vector.hpp"

#define PADDLE_LEFT 0
#define PADDLE_RIGHT 1
#define TOP_WALL 2
#define BOTTOM_WALL 3
#define LEFT_GOAL 4
#define RIGHT_GOAL 5
#define BALL 6

#define PADDLE_DRAWABLE 0
#define BALL_DRAWABLE 1

#define PI 3.141592653

struct GameState
{
  EntityList entities;
  Drawable drawables[2];
  byte scores[2];
  uint shader;
  float ballSpeed;
  float paddleSpeed;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState, float dt);

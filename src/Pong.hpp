#pragma once
#include "Platform.hpp"
#include "Entity.hpp"
#include "Vector.hpp"

#define ENTITY_COUNT 7

#define PADDLE_LEFT 0
#define PADDLE_RIGHT 1
#define TOP_WALL 2
#define BOTTOM_WALL 3
#define LEFT_WALL 4
#define RIGHT_WALL 5
#define BALL 6

#define PADDLE_DRAWABLE 0
#define BALL_DRAWABLE 1

//Paddle or ball


struct GameState
{
  Entity entities[ENTITY_COUNT];
  Drawable drawables[2];
  uint shader;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState);

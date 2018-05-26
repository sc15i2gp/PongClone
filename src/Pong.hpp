#pragma once
#include "Platform.hpp"
#include "Vector.hpp"

#define ENTITY_COUNT 7

#define PADDLE_LEFT 0
#define PADDLE_RIGHT 1
#define TOP_WALL 2
#define BOTTOM_WALL 3
#define LEFT_WALL 4
#define RIGHT_WALL 5
#define BALL 6

#define ENTITY_NULL 0
#define ENTITY_PADDLE (1 << 0)
#define ENTITY_BALL (1 << 1)
#define ENTITY_WALL (1 << 2)

#define PADDLE_DRAWABLE 0
#define BALL_DRAWABLE 1

//Paddle or ball
struct Entity
{
  uint entityType = ENTITY_NULL;
  Vec2f size = {0.0f, 0.0f};
  Vec2f velocity = {0.0f, 0.0f};
  Vec2f position = {0.0f, 0.0f};
};

struct GameState
{
  Entity entities[ENTITY_COUNT];
  Drawable drawables[2];
  uint shader;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState);

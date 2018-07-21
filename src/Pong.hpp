#pragma once
#include <cstdio>
#include "Platform.hpp"
#include "Entity.hpp"
#include "Vector.hpp"

#define PADDLE_LEFT 0
#define PADDLE_RIGHT 1
#define BALL 2
#define TOP_WALL 3
#define BOTTOM_WALL 4
#define LEFT_GOAL 5
#define RIGHT_GOAL 6

#define PADDLE_DRAWABLE 0
#define BALL_DRAWABLE 1

#define PI 3.141592653

struct Controls
{
	Key upKey;
	Key downKey;
};

struct GraphicalData
{
	uint shader;
	Drawable drawables[2];
};

struct GameData
{
	byte isPlaying;
	byte scores[2];
	uint goalScored;
};

struct GameConfig
{
  float ballSpeed;
  float paddleSpeed;
  Vec2f paddleSize;
  Vec2f ballSize;
};

struct GameState
{
  EntityList entities;
  GraphicalData graphicalData;
  GameData gameData;
  GameConfig config;
  Controls controllers[2];
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState, float dt);

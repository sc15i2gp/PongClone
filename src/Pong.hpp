#pragma once
#include <cstdio>
#include <assert.h>
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

#define EVENT_NULL 0
#define EVENT_GOAL_SCORED 1
#define EVENT_BALL_COLLIDE 2
#define EVENT_APPLY_MODIFIER 3

#define EVENT_COUNT 4

struct GameEvent
{
	uint type;
	
	union
	{
		uint goal;
		uint modifier;
	};
};

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
  GameEvent events[EVENT_COUNT];
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState, float dt);

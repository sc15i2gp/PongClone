#pragma once
#include <cstdio>
#include <assert.h>
#include "Platform.hpp"
#include "GraphicalData.hpp"
#include "Entity.hpp"
#include "Vector.hpp"
#include "Event.hpp"

#define PI 3.141592653



struct Controls
{
	Key upKey;
	Key downKey;
};

struct GameData
{
	byte isPlaying;
	byte scores[2];
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
	EventQueue events;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState, float dt);

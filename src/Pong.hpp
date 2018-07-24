#pragma once
#include <cstdio>
#include <assert.h>
#include "Platform.hpp"
#include "GraphicalData.hpp"
#include "Entity.hpp"
#include "Vector.hpp"
#include "Event.hpp"
#include "Controls.hpp"
#include "Collision.hpp"
#include "GameData.hpp"
#include "Config.hpp"

#define PI 3.141592653

#define PADDLE_X 2.5f
#define WALL_WIDTH 1.0f

struct GameState
{
  	EntityList entities;
  	GraphicalData graphicalData;
  	GameData gameData;
  	GameConfig config;
	ControllerList controllers;
	EventQueue events;
};

GameState* initGame(Platform* platform);
void gameUpdate(Platform* platform, GameState* gameState, float dt);

#pragma once
#include "Platform.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

#define PADDLE_DRAWABLE 0
#define BALL_DRAWABLE 1
#define NET_DRAWABLE 2

#define PB_SHADER 0
#define NET_SHADER 1

struct GraphicalData
{
	uint shaders[2];
	uint translationUbo;
	Drawable drawables[3];
};

void initGraphicalData(Platform* platform, GraphicalData* graphicalData, Vec2f paddleSize, Vec2f ballSize, Vec2f netSize);
void scaleGraphicalData(Platform*, GraphicalData*);
void drawEntities(EntityList* entities, GraphicalData* graphicalData);
void drawNet(GraphicalData*, Vec2f position);

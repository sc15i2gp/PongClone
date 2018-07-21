#pragma once
#include "Platform.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

#define PADDLE_DRAWABLE 0
#define BALL_DRAWABLE 1

struct GraphicalData
{
	uint shader;
	Drawable drawables[2];
};

void initGraphicalData(Platform* platform, GraphicalData* graphicalData, Vec2f paddleSize, Vec2f ballSize);
void drawEntities(EntityList* entities, GraphicalData* graphicalData);

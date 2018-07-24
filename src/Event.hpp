#pragma once
#include <cstdio>
#include <assert.h>

#include "Types.hpp"

#define EVENT_NULL 0
#define EVENT_GOAL_SCORED 1
#define EVENT_BALL_COLLIDE 2
#define EVENT_APPLY_MODIFIER 3

#define EVENT_COUNT 4

struct Event
{
	uint type;
	
	union
	{
		uint goal;
		uint modifier;
	};
};

struct EventQueue
{
	Event events[EVENT_COUNT];
};

void initEventQueue(EventQueue*);
void pushEvent(EventQueue*, uint type, uint data);

struct GameData;
struct EntityList;

typedef void (*EventHandler)(Event*, GameData*, EntityList*);

void handleEvents(EventQueue*, GameData*, EntityList*, EventHandler);

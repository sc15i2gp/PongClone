#include "Event.hpp"

void initEventQueue(EventQueue* eventQueue)
{
	for(uint e = 0; e < EVENT_COUNT; e++) eventQueue->events[e].type = EVENT_NULL;
}

void pushEvent(EventQueue* eventQueue, uint type, uint data)
{
	Event* e = eventQueue->events;
	uint i = 0;
	for(; e->type != EVENT_NULL && i < EVENT_COUNT; e++, i++);
	assert(i < EVENT_COUNT); // Queue should never be completely full at this point
	if(e->type == EVENT_GOAL_SCORED) e->goal = data;
	e->type = type;	
}

void handleEvents(EventQueue* eventQueue, GameData* gameData, EntityList* entities, EventHandler handleEvent)
{
	for(Event* e = eventQueue->events; e->type != EVENT_NULL; e++)
	{
		handleEvent(e, gameData, entities);
		e->type = EVENT_NULL;
	}
}

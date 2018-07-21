#pragma once
#include "Platform.hpp"

#define CONTROLLER_COUNT 2

struct Controller
{
	Key upKey;
	Key downKey;
	Key serveKey;
};

struct ControllerList
{
	Controller controllers[CONTROLLER_COUNT];
};

void initControllers(ControllerList*);
Controller* getController(ControllerList*, uint);
byte isUpPressed(Platform*, Controller*);
byte isDownPressed(Platform*, Controller*);
byte isServePressed(Platform*, Controller*);

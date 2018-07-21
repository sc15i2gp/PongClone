#include "Controls.hpp"

void initControllers(ControllerList* list)
{
	list->controllers[0] = {Key::W, Key::S, Key::Space};
	list->controllers[1] = {Key::I, Key::K, Key::Space};
}

Controller* getController(ControllerList* list, uint controller)
{
	return list->controllers + controller;
}

byte isUpPressed(Platform* platform, Controller* controller)
{
	return isKeyPressed(platform, controller->upKey);
}

byte isDownPressed(Platform* platform, Controller* controller)
{
	return isKeyPressed(platform, controller->downKey);
}

byte isServePressed(Platform* platform, Controller* controller)
{
	return isKeyPressed(platform, controller->serveKey);
}

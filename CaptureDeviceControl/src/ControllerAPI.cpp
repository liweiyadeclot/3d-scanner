#include "ControllerAPI.h"

static CallbackFunction g_callback = nullptr;

CONTROLLER_API void RegisterCallback(CallbackFunction callback)
{
	g_callback = callback;
	return;
}

CONTROLLER_API void Capture()
{
	return;
}
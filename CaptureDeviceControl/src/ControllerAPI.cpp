#include "ControllerAPI.h"

static CallbackFunction g_InitImagesCapturedcallback = nullptr;
static CallbackFunction g_NextImagesCapturedcallback = nullptr;

CONTROLLER_API void RegisterInitImagesCapturedCallback(CallbackFunction callback)
{
	g_InitImagesCapturedcallback = callback;
	return;
}

CONTROLLER_API void RegisterNextImagesCapturedCallback(CallbackFunction callback)
{
	g_NextImagesCapturedcallback = callback;
	return;
}

CONTROLLER_API void Capture()
{
	// when two image captured
	g_InitImagesCapturedcallback();
	// when next image captured
	g_NextImagesCapturedcallback();
	return;
}
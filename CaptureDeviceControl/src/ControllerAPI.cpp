#include "ControllerAPI.h"

static InitCallbackFunction g_InitImagesCapturedcallback = nullptr;
static NextCallbackFunction g_NextImagesCapturedcallback = nullptr;

CONTROLLER_API void RegisterInitImagesCapturedCallback(InitCallbackFunction callback)
{
	g_InitImagesCapturedcallback = callback;
	return;
}

CONTROLLER_API void RegisterNextImagesCapturedCallback(NextCallbackFunction callback)
{
	g_NextImagesCapturedcallback = callback;
	return;
}

CONTROLLER_API void Capture()
{
	// when two image captured
	// g_InitImagesCapturedcallback();
	// 
	// when next image captured
	// g_NextImagesCapturedcallback();
	return;
}
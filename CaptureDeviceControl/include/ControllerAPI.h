#pragma once

#ifdef CAPTUREDEVICECONTROL_EXPORTS
#define CONTROLLER_API __declspec(dllexport)
#else
#define CONTROLLER_API __declspec(dllimport)
#endif

typedef void (*CallbackFunction)();

// 
extern "C" CONTROLLER_API void RegisterCallback(CallbackFunction callback);

extern "C" CONTROLLER_API void Capture();
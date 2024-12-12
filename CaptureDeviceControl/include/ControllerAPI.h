#pragma once
#include <opencv2/opencv.hpp>

#ifdef CAPTUREDEVICECONTROL_EXPORTS
#define CONTROLLER_API __declspec(dllexport)
#else
#define CONTROLLER_API __declspec(dllimport)
#endif

namespace Controller
{
	typedef void (*InitCallbackFunction)(cv::Mat img1, cv::Mat img2);
	typedef void (*NextCallbackFunction)(cv::Mat img);

	// 
	extern "C" CONTROLLER_API void RegisterInitImagesCapturedCallback(InitCallbackFunction callback);
	extern "C" CONTROLLER_API void RegisterNextImagesCapturedCallback(NextCallbackFunction callback);

	extern "C" CONTROLLER_API void Capture();
}
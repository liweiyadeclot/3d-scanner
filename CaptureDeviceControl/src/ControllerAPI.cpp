#include "ControllerAPI.h"
#include "TurntableScanner.h"

static InitCallbackFunction g_InitImagesCapturedcallback = nullptr;
static NextCallbackFunction g_NextImagesCapturedcallback = nullptr;
static TurntableScanner g_scanner{};

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
	cv::Mat initImage1{ g_scanner.nextPicture() };
	cv::Mat initImage2{ g_scanner.nextPicture() };
	// when two image captured
	g_InitImagesCapturedcallback(initImage1, initImage2);
	 
	while (g_scanner.getImgCount() > 0)
	{
		cv::Mat nextImage{ g_scanner.nextPicture() };
		// when next image captured
		g_NextImagesCapturedcallback(nextImage);
	}
	return;
}
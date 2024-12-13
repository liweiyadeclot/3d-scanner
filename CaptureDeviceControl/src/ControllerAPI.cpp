#include "ControllerAPI.h"
#include "TurntableScanner.h"

namespace Controller
{
	static InitCallbackFunction g_InitImagesCapturedcallback = nullptr;
	static NextCallbackFunction g_NextImagesCapturedcallback = nullptr;
	//static TurntableScanner g_scanner{};

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
		TurntableScanner scanner{};
		while (scanner.capture() != 0);
		while (scanner.getImgCount() == 0);
		cv::Mat initImage1{ scanner.nextPicture() };
		cv::Mat initImage2{ scanner.nextPicture() };
		// when two image captured
		g_InitImagesCapturedcallback(initImage1, initImage2);

		while (scanner.getImgCount() > 0)
		{
			cv::Mat nextImage{ scanner.nextPicture() };
			// when next image captured
			g_NextImagesCapturedcallback(nextImage);
		}
		return;
	}
}
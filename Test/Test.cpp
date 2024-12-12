#include <string>
#include <thread>
#include "ControllerAPI.h"
#include "SFMAPI.h"

void OnInitTwoImageCaptured(cv::Mat img1, cv::Mat img2)
{
	std::thread sfmThread(SFM::OnInitTwoImageCaptured, img1, img2);

	sfmThread.join();
}

void OnNextImageCaptured(cv::Mat img)
{
	std::thread sfmThread(SFM::OnNextImageCaptured, img);

	sfmThread.join();
}

int main(void)
{

	Controller::RegisterInitImagesCapturedCallback(OnInitTwoImageCaptured);
	Controller::RegisterNextImagesCapturedCallback(OnNextImageCaptured);

	std::thread captureThread(Controller::Capture);

	captureThread.join();

	std::thread sfmThread(SFM::AllImageCaptured);

	sfmThread.join();
}
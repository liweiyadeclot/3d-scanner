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
	std::string path("../images/");
	cv::Mat initImage1 = cv::imread(path + std::string("0.jpg"));
	cv::Mat initImage2 = cv::imread(path + std::string("1.jpg"));

	cv::Mat img2 = cv::imread(path + std::string("2.jpg"));
	SFM::OnInitTwoImageCaptured(initImage1, initImage2);
	SFM::OnNextImageCaptured(img2);

	/*Controller::RegisterInitImagesCapturedCallback(OnInitTwoImageCaptured);
	Controller::RegisterNextImagesCapturedCallback(OnNextImageCaptured);

	std::thread captureThread(Controller::Capture);

	captureThread.join();

	std::thread sfmThread(SFM::AllImageCaptured);

	sfmThread.join();*/
}
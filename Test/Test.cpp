#include <string>
#include <thread>
#include "ControllerAPI.h"
#include "SFMAPI.h"
#include "Test.h"

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
void TestReadImageWithPath()
{
	std::string imagePath = "../images/";

	cv::Mat initImage1, initImage2;
	initImage1 = cv::imread(imagePath + "0.jpg");
	initImage2 = cv::imread(imagePath + "1.jpg");

	OnInitTwoImageCaptured(initImage1, initImage2);

	for (size_t i = 2; i < 4; ++i)
	{
		cv::Mat nextImage = cv::imread(imagePath + std::to_string(i) + ".jpg");
		OnNextImageCaptured(nextImage);
	}

	SFM::AllImageCaptured();
}


int main(void)
{
	TestReadImageWithPath();

	/*std::string imagePath = "../images/";

	Controller::RegisterInitImagesCapturedCallback(OnInitTwoImageCaptured);
	Controller::RegisterNextImagesCapturedCallback(OnNextImageCaptured);

	std::thread captureThread(Controller::Capture);

	captureThread.join();
	std::thread sfmThread(SFM::AllImageCaptured);
	sfmThread.join();*/
}


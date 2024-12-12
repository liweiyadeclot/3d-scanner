#include <string>
#include "ControllerAPI.h"
#include "SFMAPI.h"

int main(void)
{
	std::string imagePath = "../images/";

	OnInitTwoImageCaptured();

	for (size_t i = 2; i < 7; ++i)
	{
		cv::Mat newImage = cv::imread(imagePath + std::to_string(i) + ".jpg");
		OnNextImageCaptured(newImage);
	}

	AllImageCaptured();
}
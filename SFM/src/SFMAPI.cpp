#include "SFMAPI.h"
#include "OutputPLY.h"

namespace SFM
{
	SFMAPI void OnInitTwoImageCaptured(cv::Mat img1, cv::Mat img2)
	{
		InitConstruction(img1, img2);
		subImageBag.push_back(initImages[1]);
	}

	SFMAPI void OnNextImageCaptured(cv::Mat newImage)
	{
		AddImage(newImage);
	}


	SFMAPI void AllImageCaptured()
	{
		OutputPLY(allReconstructedPoints, allPointsColors);
	}

	SFMAPI void SetImagePath(const std::string& path)
	{
		INIT_IMG_PATH1 = path + "0.jpg";
		INIT_IMG_PATH2 = path + "1.jpg";
	}

}
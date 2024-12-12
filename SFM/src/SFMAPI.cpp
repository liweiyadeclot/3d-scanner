#include "SFMAPI.h"
#include "OutputPLY.h"

SFMAPI void OnInitTwoImageCaptured()
{
	SFM::InitConstruction();
	SFM::subImageBag.push_back(SFM::initImages[1]);
}

SFMAPI void OnNextImageCaptured(cv::Mat newImage)
{
	SFM::AddImage(newImage);
}


SFMAPI void AllImageCaptured()
{
	OutputPLY(SFM::allReconstructedPoints, SFM::allPointsColors);
}

SFMAPI void SetImagePath(const std::string& path)
{
	SFM::INIT_IMG_PATH1 = path + "0.jpg";
	SFM::INIT_IMG_PATH2 = path + "1.jpg";
}

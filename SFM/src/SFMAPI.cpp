#include "SFMAPI.h"
#include "OutputPLY.h"
#include "OutputToFile.h"

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
		std::vector<CameraInfo> cameraInfos;
		std::vector<ImageData> imageDatas;
		std::vector<Point3dInfo> point3ds;
		PrepareDataToFile(cameraInfos, imageDatas, point3ds);
		OutPutToFile(imageDatas, cameraInfos, point3ds);
	}

	SFMAPI void SetImagePath(const std::string& path)
	{

	}

}
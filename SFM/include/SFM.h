#pragma once
#include "Image.h"
#include "Constructor.h"
#include "DataInfoForSave.h"

namespace SFM
{

	extern std::string INIT_IMG_PATH1;

	extern std::string INIT_IMG_PATH2;

	extern std::vector<Image> initImages;
	extern std::vector<cv::Point3d> allReconstructedPoints;
	extern std::vector<cv::Vec3b> allPointsColors;

	extern std::vector<std::string> subImagePaths;
	extern std::vector<Image> subImageBag;

	void InitConstruction(cv::Mat img1, cv::Mat img2);

	void AddImageToConstruction();

	cv::Mat Matrix2Quaternion(cv::Mat matrix);

	void OutPutToFile(const std::vector<ImageData> imgs, std::vector<CameraInfo> cmrs, const std::vector<Point3dInfo> p3ds);

	void AddImage(cv::Mat newImage);
}
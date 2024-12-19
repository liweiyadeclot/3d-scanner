#pragma once
#include "Image.h"
#include "Constructor.h"
#include "DataInfoForSave.h"

namespace SFM
{
	extern std::vector<Image> initImages;
	extern std::vector<cv::Point3d> allReconstructedPoints;
	extern std::vector<cv::Vec3b> allPointsColors;

	extern std::vector<std::string> subImagePaths;
	extern std::vector<Image> subImageBag;
	extern const cv::Mat K;

	void InitConstruction(cv::Mat img1, cv::Mat img2);

	void AddImageToConstruction();


	void OutPutToFile(const std::vector<ImageData> imgs, std::vector<CameraInfo> cmrs, const std::vector<Point3dInfo> p3ds);

	void AddImage(cv::Mat newImage);

	std::vector<double> Matrix2Quaternion(cv::Mat matrix);
	std::vector<double> RodriguesToQuaternion(const cv::Mat& rvec);

}
#include "Constructor.h"

void Constructor::FindCamera(const cv::Mat& K,  std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2, cv::Mat& outputR, cv::Mat& outputT, std::vector<uchar>& mask)
{
	std::vector<uchar> inliers;

	cv::Mat essencialMat;
	essencialMat = cv::findEssentialMat(points1, points2, K, cv::RANSAC, 0.6, 1.0, inliers);

	mask = inliers;

	// 根据内点筛选出新的匹配点
	Constructor::MaskoutPoint(points1, inliers);
	Constructor::MaskoutPoint(points2, inliers);

	cv::recoverPose(essencialMat, points1, points2, K, outputR, outputT);
}

void Constructor::MaskoutPoint(std::vector<cv::Point2f>& inputPoints, const std::vector<uchar>& inputMask)
{
	std::vector<cv::Point2f> tempPoints(inputPoints);
	inputPoints.clear();

	for (size_t i = 0; i < tempPoints.size(); ++i)
	{
		if (inputMask[i])
		{
			inputPoints.push_back(tempPoints[i]);
		}
	}
}

// Get 3d location of the same points of two camera.
std::vector<cv::Point3d> Constructor::PointsReconstruct(const cv::Mat& K, const cv::Mat& R1, 
	const cv::Mat& T1, const cv::Mat& R2,
	const cv::Mat& T2, std::vector<cv::Point2f>& points1,
	std::vector<cv::Point2f>& points2)
{
	cv::Mat proj1(3, 4, CV_32FC1);
	cv::Mat proj2(3, 4, CV_32FC1);

	R1.convertTo(proj1(cv::Range(0, 3), cv::Range(0, 3)), CV_32FC1);
	T1.convertTo(proj1.col(3), CV_32FC1);

	R2.convertTo(proj2(cv::Range(0, 3), cv::Range(0, 3)), CV_32FC1);
	T2.convertTo(proj2.col(3), CV_32FC1);

	cv::Mat fK;
	K.convertTo(fK, CV_32FC1);
	proj1 = fK * proj1;
	proj2 = fK * proj2;

	// triangulate
	cv::Mat points4D(4, points1.size(), CV_32F);
	cv::triangulatePoints(proj1, proj2, points1, points2, points4D);
	std::vector<cv::Point3d> points3D;
	for (size_t i = 0; i < points4D.cols; ++i)
	{
		float W = points4D.at<float>(3, i);
		if (W == 0.0f)
		{
			std::cout << "Error! Divided by zero!" << std::endl;
		}
		float X = points4D.at<float>(0, i) / W;
		float Y = points4D.at<float>(1, i) / W;
		float Z = points4D.at<float>(2, i) / W;
		points3D.push_back(cv::Point3d(X, Y, Z));
	}

	return points3D;
}


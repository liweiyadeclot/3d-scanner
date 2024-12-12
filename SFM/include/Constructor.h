#pragma once
#include <opencv2/opencv.hpp>
#include "Image.h"

class Constructor
{
public:
	static void FindCamera(const cv::Mat& K,  std::vector<cv::Point2f >& points1, std::vector<cv::Point2f>& points2, cv::Mat& outputR, cv::Mat& outputT, std::vector<uchar>& mask);

	// 输入图匹配点，内点标记mask，返回mask后的vector<Point2f>匹配点
	static void MaskoutPoint(std::vector<cv::Point2f>& inputPoints, const std::vector<uchar>& inputMask);

	// 输入图一的R,T匹配点，图二的R,T匹配点，返回三维点
	static std::vector<cv::Point3d> PointsReconstruct(const cv::Mat& K,const cv::Mat& R1, const cv::Mat& T1, const cv::Mat& R2, const cv::Mat& T2, std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2);
};
#pragma once
#include <opencv2/opencv.hpp>
#include "Image.h"

class Constructor
{
public:
	static void FindCamera(const cv::Mat& K,  std::vector<cv::Point2f >& points1, std::vector<cv::Point2f>& points2, cv::Mat& outputR, cv::Mat& outputT, std::vector<uchar>& mask);

	// ����ͼƥ��㣬�ڵ���mask������mask���vector<Point2f>ƥ���
	static void MaskoutPoint(std::vector<cv::Point2f>& inputPoints, const std::vector<uchar>& inputMask);

	// ����ͼһ��R,Tƥ��㣬ͼ����R,Tƥ��㣬������ά��
	static std::vector<cv::Point3d> PointsReconstruct(const cv::Mat& K,const cv::Mat& R1, const cv::Mat& T1, const cv::Mat& R2, const cv::Mat& T2, std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2);
};
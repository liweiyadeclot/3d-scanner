#pragma once
#include <opencv2/opencv.hpp>

static int s_numImg = 0;

class Image
{
public:
	Image(const std::string& path);
	Image(const cv::Mat& img);
	void MatchFeatures(Image& otherImage,  std::vector<cv::DMatch>& outputMatches);

	void FindColor();

	void GetObjPointsAndImagePoints(const std::vector<cv::DMatch>& matches, std::vector<cv::Point3d>& allReconstructedPoints, Image& preImage);
public:
	// 匹配点所对应的空间点在点云中的索引 i-1 和 i匹配
	std::vector<int> m_correspondStructIdx;
	// i-1 和 i 匹配得到的匹配点, 在重建完成后是经过Ransac优化的
	std::vector<cv::Point2f> m_matchedPoints;
	cv::Mat m_R;
	cv::Mat m_t;
	std::vector<cv::Vec3b> m_colors;

	// 前一张图中匹配点对应的三维点
	std::vector<cv::Point3f> m_objectPoints;
	// 在现图像中对应的像素点
	std::vector<cv::Point2f> m_imagePoints;

private:
	cv::Mat m_image;
	std::vector<cv::KeyPoint> m_keyPoints;
	cv::Mat m_descriptor;


	int m_Index;
};
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
	// ƥ�������Ӧ�Ŀռ���ڵ����е����� i-1 �� iƥ��
	std::vector<int> m_correspondStructIdx;
	// i-1 �� i ƥ��õ���ƥ���, ���ؽ���ɺ��Ǿ���Ransac�Ż���
	std::vector<cv::Point2f> m_matchedPoints;
	cv::Mat m_R;
	cv::Mat m_t;
	std::vector<cv::Vec3b> m_colors;

	// ǰһ��ͼ��ƥ����Ӧ����ά��
	std::vector<cv::Point3f> m_objectPoints;
	// ����ͼ���ж�Ӧ�����ص�
	std::vector<cv::Point2f> m_imagePoints;

private:
	cv::Mat m_image;
	std::vector<cv::KeyPoint> m_keyPoints;
	cv::Mat m_descriptor;


	int m_Index;
};
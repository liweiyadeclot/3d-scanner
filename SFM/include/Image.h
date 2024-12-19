#pragma once
#include <opencv2/opencv.hpp>

static int s_numImg = 0;

class Image
{
public:

	int m_nfeatures = 0;
	int m_nOctaveLayers = 3;
	double m_contrastThreshold = 0.04;
	double m_edgeThreshold = 10;

	Image(const cv::Mat& img);
	void MatchFeatures(Image& otherImage,  std::vector<cv::DMatch>& outputMatches);

	void FindColor();

	void GetObjPointsAndImagePoints(const std::vector<cv::DMatch>& matches, std::vector<cv::Point3d>& allReconstructedPoints, Image& preImage);

	void SaveJPG();
public:
	
	std::vector<int> m_correspondStructIdx;
	std::vector<cv::Point2f> m_matchedPoints;
	cv::Mat m_R;
	cv::Mat m_t;
	std::vector<cv::Vec3b> m_colors;


	std::vector<cv::Point3f> m_objectPoints;

	std::vector<cv::Point2f> m_imagePoints;

	std::vector<cv::KeyPoint> m_keyPoints;
	cv::Mat m_image;
	std::string m_imageName;
	int m_Index;
	uint8_t* m_imageData;
	uint32_t m_imageDataSize;
private:
	int GetFileSize(const std::string& path);
private:
	cv::Mat m_descriptor;

};
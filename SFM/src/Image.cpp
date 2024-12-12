#include "Image.h"
#include <cassert>
Image::Image(const std::string & path)
{
	m_image = cv::imread(path);
	assert(!m_image.empty());
	

	s_numImg++;
	m_Index = s_numImg;

	// SIFT
	//cv::Ptr<cv::SIFT> sift = cv::SIFT::create(0, 10, 0.000001, 16);
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create(0, 3, 0.1, 10);
	sift->detectAndCompute(m_image, cv::noArray(), m_keyPoints, m_descriptor);
	cv::Mat outputKeypoints;
	cv::drawKeypoints(m_image, m_keyPoints, outputKeypoints);
	cv::imwrite("KeyPoints" + std::to_string(m_Index) + ".jpg", outputKeypoints);
	for (size_t i = 0; i < m_keyPoints.size(); ++i)
	{
		m_correspondStructIdx.push_back(-1);
	}

	
}

Image::Image(const cv::Mat& img) : m_image(img)
{
	assert(!m_image.empty());
	s_numImg++;
	m_Index = s_numImg;

	// SIFT
	//cv::Ptr<cv::SIFT> sift = cv::SIFT::create(0, 10, 0.000001, 16);
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create(0, 3, 0.1, 10);
	sift->detectAndCompute(m_image, cv::noArray(), m_keyPoints, m_descriptor);
	cv::Mat outputKeypoints;
	cv::drawKeypoints(m_image, m_keyPoints, outputKeypoints);
	cv::imwrite("KeyPoints" + std::to_string(m_Index) + ".jpg", outputKeypoints);
	for (size_t i = 0; i < m_keyPoints.size(); ++i)
	{
		m_correspondStructIdx.push_back(-1);
	}
}

void Image::MatchFeatures(Image& otherImage, std::vector<cv::DMatch>& outputMatches)
{
	otherImage.m_matchedPoints.clear();
	m_matchedPoints.clear();

	std::vector<std::vector<cv::DMatch>>  matches;
	cv::FlannBasedMatcher matcher;
	matcher.knnMatch(m_descriptor, otherImage.m_descriptor, matches, 2);


	
	for (size_t i = 0; i < matches.size(); ++i)
	{
		if (matches[i][0].distance < 0.8f * matches[i][1].distance)
		{
			outputMatches.push_back(matches[i][0]);
		}
	}

	for (size_t i = 0; i < outputMatches.size(); ++i)
	{
		m_matchedPoints.push_back(m_keyPoints[outputMatches[i].queryIdx].pt);
		otherImage.m_matchedPoints.push_back(otherImage.m_keyPoints[outputMatches[i].trainIdx].pt);
	}

}

void Image::FindColor()
{
	for (cv::Point2f& point : m_matchedPoints)
	{
		cv::Vec3b color = m_image.at<cv::Vec3b>(point);
		m_colors.push_back(color);
	}
}

void Image::GetObjPointsAndImagePoints(const std::vector<cv::DMatch>& matches, std::vector<cv::Point3d>& allReconstructedPoints, Image& preImage)
{
	m_objectPoints.clear();
	m_imagePoints.clear();

	for (size_t i = 0; i < matches.size(); ++i)
	{
		// 获取匹配点在preImage中对应的三维空间点的索引
		int matchedWorldPoitnIndices = preImage.m_correspondStructIdx[matches[i].queryIdx];
		// 如果匹配点在preImage对应的三维空间点存在
		if (matchedWorldPoitnIndices > 0)
		{
			// 将其（前一张图像中的三维点添加）
			// TO-DO: We should re caculate the new point 3d pos and get average value for the 3d point.
			m_objectPoints.push_back(allReconstructedPoints[matchedWorldPoitnIndices]);

			// 将匹配点（新图像的二维点)添加
			m_imagePoints.push_back(m_keyPoints[matches[i].trainIdx].pt);
		}
	}
}


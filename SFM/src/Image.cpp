#include "Image.h"
#include <cassert>
#include <fstream>

const std::string OutPutPath ="../Captured/";

Image::Image(const cv::Mat& img) : m_image(img)
{
	assert(!m_image.empty());
	SaveJPG();

	s_numImg++;
	m_Index = s_numImg;

	// SIFT
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create(m_nfeatures, m_nOctaveLayers, m_contrastThreshold, m_edgeThreshold);
	// cv::Ptr<cv::SIFT> sift = cv::SIFT::create(0, 3, 0.1, 10);
	sift->detectAndCompute(m_image, cv::noArray(), m_keyPoints, m_descriptor);
	cv::Mat outputKeypoints;
	cv::drawKeypoints(m_image, m_keyPoints, outputKeypoints);
	cv::imwrite("KeyPoints" + std::to_string(m_Index) + ".jpg", outputKeypoints);
	for (size_t i = 0; i < m_keyPoints.size(); ++i)
	{
		m_correspondStructIdx.push_back(-1);
	}
}

int Image::GetFileSize(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
	{
		// 如果文件无法打开，返回0
		return 0;
	}

	// 移动到文件末尾
	file.seekg(0, std::ios::end);

	// 获取文件大小
	int fileSize = static_cast<size_t>(file.tellg());

	// 关闭文件
	file.close();

	// 返回文件大小
	return fileSize;
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
		// ��ȡƥ�����preImage�ж�Ӧ����ά�ռ�������
		int matchedWorldPoitnIndices = preImage.m_correspondStructIdx[matches[i].queryIdx];
		// ���ƥ�����preImage��Ӧ����ά�ռ�����
		if (matchedWorldPoitnIndices > 0)
		{
		// TO-DO: We should re caculate the new point 3d pos and get average value for the 3d point.
			m_objectPoints.push_back(allReconstructedPoints[matchedWorldPoitnIndices]);

			m_imagePoints.push_back(m_keyPoints[matches[i].trainIdx].pt);
		}
	}
}

void Image::SaveJPG()
{
	std::string fileName(OutPutPath + "Captured" + std::to_string(s_numImg) + ".jpg");

	cv::imwrite(fileName, m_image);
	std::ifstream file(fileName, std::ios::binary);
	size_t size = GetFileSize(fileName);
	m_imageDataSize = size;
	m_imageData = new uint8_t[size];
	file.read(reinterpret_cast<char*>(m_imageData), size);
}


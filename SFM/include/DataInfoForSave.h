#pragma once
#pragma warning(disable : 4996)

#include <cstdint>
#include <vector>
#include <cstring>
typedef struct CameraInfo
{
	char* m_type;
	double* m_params;
	uint32_t m_id;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_paramCount;

	CameraInfo()
		: m_type(nullptr), m_params(nullptr)
	{
	}

	CameraInfo(const CameraInfo& other)
	{
		m_type = new char[strlen(other.m_type) + 1];
		strcpy(m_type, other.m_type);
		m_params = new double[other.m_paramCount];
		for (uint32_t i = 0; i < other.m_paramCount; i++)
		{
			m_params[i] = other.m_params[i];
		}
		m_id = other.m_id;
		m_width = other.m_width;
		m_height = other.m_height;
		m_paramCount = other.m_paramCount;
	}

	CameraInfo& operator=(const CameraInfo& other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (m_type != nullptr)
		{
			delete[] m_type;
		}
		if (m_params != nullptr)
		{
			delete[] m_params;
		}
		m_type = new char[strlen(other.m_type) + 1];
		strcpy(m_type, other.m_type);
		m_params = new double[other.m_paramCount];
		for (uint32_t i = 0; i < other.m_paramCount; i++)
		{
			m_params[i] = other.m_params[i];
		}
		m_id = other.m_id;
		m_width = other.m_width;
		m_height = other.m_height;
		m_paramCount = other.m_paramCount;
		return *this;
	}

	~CameraInfo() {
		delete[] m_type;
		delete[] m_params;
	}

} CameraInfo;

typedef struct Point2dInfo
{
	double m_x;
	double m_y;
	int m_p3dId;
} Point2dInfo;

typedef struct TrackInfo
{
	uint32_t m_imageId;
	uint32_t m_point2dIdx;
} TrackInfo;

typedef struct Point3dInfo
{
	TrackInfo* m_tracks;
	double m_x;
	double m_y;
	double m_z;
	double m_reprojectionError;
	uint32_t m_id;
	uint32_t m_trackCount;
	uint8_t m_colorR;
	uint8_t m_colorG;
	uint8_t m_colorB;

	Point3dInfo()
		: m_tracks(nullptr), m_x(0), m_y(0), m_z(0), m_reprojectionError(0), m_id(0), m_trackCount(0), m_colorR(0), m_colorG(0), m_colorB(0)
	{
	}

	Point3dInfo(const Point3dInfo& other)
	{
		m_tracks = new TrackInfo[other.m_trackCount];
		for (uint32_t i = 0; i < other.m_trackCount; i++)
		{
			m_tracks[i] = other.m_tracks[i];
		}
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
		m_reprojectionError = other.m_reprojectionError;
		m_id = other.m_id;
		m_trackCount = other.m_trackCount;
		m_colorR = other.m_colorR;
		m_colorG = other.m_colorG;
		m_colorB = other.m_colorB;
	}

	Point3dInfo& operator=(const Point3dInfo& other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (m_tracks != nullptr)
		{
			delete[] m_tracks;
		}
		m_tracks = new TrackInfo[other.m_trackCount];
		for (uint32_t i = 0; i < other.m_trackCount; i++)
		{
			m_tracks[i] = other.m_tracks[i];
		}
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
		m_reprojectionError = other.m_reprojectionError;
		m_id = other.m_id;
		m_trackCount = other.m_trackCount;
		m_colorR = other.m_colorR;
		m_colorG = other.m_colorG;
		m_colorB = other.m_colorB;
		return *this;
	}

	~Point3dInfo() {
		if (m_tracks != nullptr)
		{
			delete[] m_tracks;
		}
	}
} Point3dInfo;

typedef struct ImageData
{
	Point2dInfo* m_points2d;
	char* m_name;
	uint8_t* m_imageData;
	double m_quaternion[4];
	double m_translation[3];
	uint32_t m_id;
	uint32_t m_cameraId;
	uint32_t m_point2dCount;
	uint32_t m_imageDataSize;

	ImageData()
		: m_points2d(nullptr), m_name(nullptr), m_imageData(nullptr), m_id(0), m_cameraId(0), m_point2dCount(0), m_imageDataSize(0)
	{
	}

	ImageData(const ImageData& other)
		: m_points2d(nullptr), m_name(nullptr), m_imageData(nullptr), m_id(other.m_id), m_cameraId(other.m_cameraId), m_point2dCount(other.m_point2dCount), m_imageDataSize(other.m_imageDataSize)
	{
		m_points2d = new Point2dInfo[other.m_point2dCount];
		for (uint32_t i = 0; i < other.m_point2dCount; i++)
		{
			m_points2d[i] = other.m_points2d[i];
		}
		m_name = new char[strlen(other.m_name) + 1];
		strcpy(m_name, other.m_name);
		for (uint32_t i = 0; i < 4; i++)
		{
			m_quaternion[i] = other.m_quaternion[i];
		}
		for (uint32_t i = 0; i < 3; i++)
		{
			m_translation[i] = other.m_translation[i];
		}
		m_imageData = new uint8_t[other.m_imageDataSize];
		for (uint32_t i = 0; i < other.m_imageDataSize; i++)
		{
			m_imageData[i] = other.m_imageData[i];
		}
	}

	ImageData& operator=(const ImageData& other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (m_points2d != nullptr)
		{
			delete[] m_points2d;
		}
		if (m_name != nullptr)
		{
			delete[] m_name;
		}
		if (m_imageData != nullptr)
		{
			delete[] m_imageData;
		}
		m_points2d = new Point2dInfo[other.m_point2dCount];
		for (uint32_t i = 0; i < other.m_point2dCount; i++)
		{
			m_points2d[i] = other.m_points2d[i];
		}
		m_name = new char[strlen(other.m_name) + 1];
		strcpy(m_name, other.m_name);
		for (uint32_t i = 0; i < 4; i++)
		{
			m_quaternion[i] = other.m_quaternion[i];
		}
		for (uint32_t i = 0; i < 3; i++)
		{
			m_translation[i] = other.m_translation[i];
		}
		for (uint32_t i = 0; i < other.m_imageDataSize; i++)
		{
			m_imageData[i] = other.m_imageData[i];
		}
		m_id = other.m_id;
		m_cameraId = other.m_cameraId;
		m_point2dCount = other.m_point2dCount;
		m_imageDataSize = other.m_imageDataSize;
		return *this;
	}

	~ImageData() {
		if (m_points2d != nullptr)
		{
			delete[] m_points2d;
		}
		if (m_name != nullptr)
		{
			delete[] m_name;
		}
		if (m_imageData != nullptr)
		{
			delete[] m_imageData;
		}
	}
} ImageData;
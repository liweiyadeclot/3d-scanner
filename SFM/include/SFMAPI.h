#pragma once

#include <string>
#include "SFM.h"

#ifdef SFM_EXPORTS
#define SFMAPI __declspec(dllexport)
#else
#define SFMAPI __declspec(dllimport)
#endif


namespace SFM
{
	extern "C" SFMAPI void OnInitTwoImageCaptured(cv::Mat img1, cv::Mat img2);

	extern "C" SFMAPI void OnNextImageCaptured(cv::Mat newImage);

	extern "C" SFMAPI void AllImageCaptured();

	extern "C" SFMAPI void SetImagePath(const std::string & path);
}
#pragma once

#include <string>
#include "SFM.h"

#ifdef SFM_EXPORTS
#define SFMAPI __declspec(dllexport)
#else
#define SFMAPI __declspec(dllimport)
#endif



extern "C" SFMAPI void OnInitTwoImageCaptured();

extern "C" SFMAPI void OnNextImageCaptured(cv::Mat newImage);

extern "C" SFMAPI void AllImageCaptured();

extern "C" SFMAPI void SetImagePath(const std::string& path);
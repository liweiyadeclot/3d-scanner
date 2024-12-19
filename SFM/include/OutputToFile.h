#pragma once
#include "SFM.h"

namespace SFM
{


	void PrepareDataToFile(std::vector<CameraInfo>& cameraInfos,
		std::vector<ImageData>& imageDatas, std::vector<Point3dInfo>& point3ds);
	
}

#include "OutputPLY.h"

void OutputPLY(std::vector<cv::Point3d>& allReconstructedPoints, std::vector<cv::Vec3b>& allPointsColors)
{
	std::ofstream plyFile(PLY_SAVE_PATH);
	plyFile << "ply\n";
	plyFile << "format ascii 1.0\n";
	plyFile << "element vertex " << allReconstructedPoints.size() << "\n";
	plyFile << "property float x\n";
	plyFile << "property float y\n";
	plyFile << "property float z\n";
	plyFile << "property uchar blue\n";
	plyFile << "property uchar green\n";
	plyFile << "property uchar red\n";
	plyFile << "end_header\n";

	for (size_t i = 0; i < allReconstructedPoints.size(); ++i)
	{
		cv::Vec3b color = allPointsColors[i];
		cv::Point3f point = allReconstructedPoints[i];
		plyFile << point.x << " " << point.y << " " << point.z << " "
			<< static_cast<int>(color[0]) << " "
			<< static_cast<int>(color[1]) << " "
			<< static_cast<int>(color[2]) << std::endl;
	}
	plyFile.close();

	std::cout << "PLY Completed!" << std::endl;
}
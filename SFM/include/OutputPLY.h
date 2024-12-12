#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>

static const std::string PLY_SAVE_PATH = "../images/output.ply";

void OutputPLY(std::vector<cv::Point3d>& allReconstructedPoints, std::vector<cv::Vec3b>& allPointsColors);



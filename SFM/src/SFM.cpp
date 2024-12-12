#include <fstream>
#include "SFM.h"


namespace SFM
{

	std::vector<Image> initImages;
	std::vector<cv::Point3d> allReconstructedPoints;
	std::vector<cv::Vec3b> allPointsColors;

	std::vector<std::string> subImagePaths;
	std::vector<Image> subImageBag;

	const cv::Mat K = (cv::Mat_<double>(3, 3) << 2905.88f, 0.0f, 1416.0f,
		0.0f, 2905.88f, 1064.7f,
		0.0f, 0.0f, 1.0f);

	void InitConstruction(cv::Mat img1, cv::Mat img2)
	{

		initImages.push_back(Image(img1));
		initImages.push_back(Image(img2));

		std::cout << initImages.size() << std::endl;
		std::vector<cv::DMatch> matches;
		initImages[0].MatchFeatures(initImages[1], matches);

		std::vector<uchar> mask;
		Constructor::FindCamera(K, initImages[0].m_matchedPoints, initImages[1].m_matchedPoints, initImages[1].m_R,
			initImages[1].m_t, mask);

		initImages[0].m_R = cv::Mat::eye(3, 3, CV_64FC1);
		initImages[0].m_t = cv::Mat::zeros(3, 1, CV_64FC1);

		allReconstructedPoints = Constructor::PointsReconstruct(K, initImages[0].m_R, initImages[0].m_t,
			initImages[1].m_R, initImages[1].m_t,
			initImages[0].m_matchedPoints,
			initImages[1].m_matchedPoints);

		initImages[1].FindColor();
		for (size_t i = 0; i < initImages[1].m_colors.size(); ++i)
		{
			allPointsColors.push_back(initImages[1].m_colors[i]);
		}


		int idx = 0;
		for (size_t i = 0; i < matches.size(); ++i)
		{
			if (mask[i])
			{
				initImages[0].m_correspondStructIdx[matches[i].queryIdx] = idx;
				initImages[1].m_correspondStructIdx[matches[i].trainIdx] = idx;
				idx++;
			}
		}
	}

	void AddImageToConstruction()
	{
		int newImageIndex = subImageBag.size() - 1;
		for (size_t j = 1; j <= newImageIndex; j++)
		{
			size_t preImageIndex = newImageIndex - j;

			std::vector<cv::DMatch> matches;
			subImageBag[preImageIndex].MatchFeatures(subImageBag[newImageIndex], matches);

			subImageBag[newImageIndex].GetObjPointsAndImagePoints(matches, allReconstructedPoints, subImageBag[preImageIndex]);

			// Ransac for mask
			std::vector<uchar> mask;
			cv::Mat discardR, discardT;
			Constructor::FindCamera(K, subImageBag[preImageIndex].m_matchedPoints, subImageBag[newImageIndex].m_matchedPoints, discardR, discardT, mask);

			// TO-DO: Why dont use discardR and discardT?
			cv::solvePnPRansac(subImageBag[newImageIndex].m_objectPoints, subImageBag[newImageIndex].m_imagePoints, K, cv::noArray(), subImageBag[newImageIndex].m_R, subImageBag[newImageIndex].m_t);

			cv::Rodrigues(subImageBag[newImageIndex].m_R, subImageBag[newImageIndex].m_R);

			std::vector<cv::Point3d> newRestructurePoints;
			newRestructurePoints = Constructor::PointsReconstruct(K, subImageBag[preImageIndex].m_R, subImageBag[preImageIndex].m_t,
				subImageBag[newImageIndex].m_R, subImageBag[newImageIndex].m_t,
				subImageBag[preImageIndex].m_matchedPoints,
				subImageBag[newImageIndex].m_matchedPoints);

			subImageBag[newImageIndex].FindColor();

			int idx = 0;
			std::vector<bool> isExsited(newRestructurePoints.size(), false);
			int pointMasked = 0;
			for (size_t k = 0; k < matches.size(); ++k)
			{
				if (mask[k])
				{
					if (subImageBag[preImageIndex].m_correspondStructIdx[matches[k].queryIdx] != -1)
					{
						// Point exsited
						subImageBag[newImageIndex].m_correspondStructIdx[matches[k].trainIdx] =
							subImageBag[preImageIndex].m_correspondStructIdx[matches[k].queryIdx];

						isExsited[k - pointMasked] = true;

					}
					else
					{
						// New point
						subImageBag[preImageIndex].m_correspondStructIdx[matches[k].queryIdx] = allReconstructedPoints.size() + idx;
						subImageBag[newImageIndex].m_correspondStructIdx[matches[k].trainIdx] = allReconstructedPoints.size() + idx;
						idx++;
					}
				}
				else
				{
					pointMasked++;
				}

			}

			for (size_t k = 0; k < newRestructurePoints.size(); ++k)
			{
				if (!isExsited[k])
				{
					allReconstructedPoints.push_back(newRestructurePoints[k]);
					allPointsColors.push_back(subImageBag[newImageIndex].m_colors[k]);
				}
			}
		}
		
	}

	cv::Mat Matrix2Quaternion(cv::Mat matrix)
	{
		float tr, qx, qy, qz, qw;

		// ¼ÆËã¾ØÕó¹ì¼£
		float a[4][4] = { 0 };
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = matrix.at<double>(i, j);

		// I removed + 1.0f; see discussion with Ethan
		float trace = a[0][0] + a[1][1] + a[2][2];
		if (trace > 0)
		{
			// I changed M_EPSILON to 0
			float s = 0.5f / sqrtf(trace + 1.0f);
			qw = 0.25f / s;
			qx = (a[2][1] - a[1][2]) * s;
			qy = (a[0][2] - a[2][0]) * s;
			qz = (a[1][0] - a[0][1]) * s;
		}
		else
		{
			if (a[0][0] > a[1][1] && a[0][0] > a[2][2])
			{
				float s = 2.0f * sqrtf(1.0f + a[0][0] - a[1][1] - a[2][2]);
				qw = (a[2][1] - a[1][2]) / s;
				qx = 0.25f * s;
				qy = (a[0][1] + a[1][0]) / s;
				qz = (a[0][2] + a[2][0]) / s;
			}
			else if (a[1][1] > a[2][2])
			{
				float s = 2.0f * sqrtf(1.0f + a[1][1] - a[0][0] - a[2][2]);
				qw = (a[0][2] - a[2][0]) / s;
				qx = (a[0][1] + a[1][0]) / s;
				qy = 0.25f * s;
				qz = (a[1][2] + a[2][1]) / s;
			}
			else
			{
				float s = 2.0f * sqrtf(1.0f + a[2][2] - a[0][0] - a[1][1]);
				qw = (a[1][0] - a[0][1]) / s;
				qx = (a[0][2] + a[2][0]) / s;
				qy = (a[1][2] + a[2][1]) / s;
				qz = 0.25f * s;
			}
		}

		float q[] = { qw, qx, qy, qz };
		// cout<< "\n quaternion:"<<cv::Mat(4,1,CV_32FC1,q).t()<<endl;
		return cv::Mat(4, 1, CV_32FC1, q).clone();
	}

	void OutPutToFile(const std::vector<ImageData> imgs, std::vector<CameraInfo> cmrs, const std::vector<Point3dInfo> p3ds)
	{
		std::ofstream cameraFile("../images/cameras.txt");
		std::ofstream imageFile("../images/images.txt");
		std::ofstream p3dFile("../images/points3D.txt");
		// CameraInfo
		for (size_t i = 0; i < cmrs.size(); ++i)
		{
			cameraFile << cmrs[i].m_id << " " << cmrs[i].m_width << " " << cmrs[i].m_height
				<< " " << cmrs[i].m_params[0] << " " << cmrs[i].m_params[1] << " " << cmrs[i].m_params[2]
				<< " " << cmrs[i].m_params[3] << "\n";
		}
		cameraFile.close();
		// ImageInfo
		for (size_t i = 0; i < imgs.size(); ++i)
		{
			imageFile << imgs[i].m_id << " " << imgs[i].m_quaternion[0] << " "
				<< imgs[i].m_quaternion[1] << " " << imgs[i].m_quaternion[2] << " "
				<< imgs[i].m_quaternion[3] << " " << imgs[i].m_translation[0] << " "
				<< imgs[i].m_translation[1] << " " << imgs[i].m_translation[2] << " "
				<< imgs[i].m_cameraId << " " << imgs[i].m_name << "\n";
			for (size_t k = 0; k < imgs[i].m_point2dCount; ++k)
			{
				imageFile << imgs[i].m_points2d[k].m_x << " " << imgs[i].m_points2d[k].m_y << " " << imgs[i].m_points2d[k].m_p3dId << " ";
			}
			imageFile << "\n";
		}
		imageFile.close();
		// Point3dInfo
		for (size_t i = 0; i < p3ds.size(); ++i)
		{
			p3dFile << p3ds[i].m_id << " " << p3ds[i].m_x << " " << p3ds[i].m_y << " " << p3ds[i].m_z
				<< " " << static_cast<int>(p3ds[i].m_colorR) << " "
				<< static_cast<int>(p3ds[i].m_colorG) << " " << static_cast<int>(p3ds[i].m_colorB) << " "
				<< p3ds[i].m_reprojectionError << " ";
			for (size_t k = 0; k < p3ds[i].m_trackCount; ++k)
			{
				p3dFile << p3ds[i].m_tracks[k].m_imageId << " " << p3ds[i].m_tracks[k].m_point2dIdx << " ";
			}
			p3dFile << "\n";

		}
		p3dFile.close();
	}

	void AddImage(cv::Mat newImage)
	{
		subImageBag.push_back(Image(newImage));
		AddImageToConstruction();
	}
}
#include <OutputToFile.h>

namespace SFM
{

	void PrepareDataToFile(std::vector<CameraInfo>& cameraInfos,
		std::vector<ImageData>& imageDatas, std::vector<Point3dInfo>& point3ds)
	{
		{
			// CameraInfo
			for (size_t i = 0; i < initImages.size(); ++i)
			{
				CameraInfo t;
				t.m_id = i;
				t.m_type = new char[sizeof("PINHOLE")];
				strcpy(t.m_type, "PINHOLE");

				t.m_paramCount = 4;
				t.m_width = initImages[0].m_image.cols;
				t.m_height = initImages[0].m_image.rows;

				t.m_params = new double[t.m_paramCount];
				// t.m_params[0] = 5465.423f;
				// t.m_params[1] = 5494.0f;
				// t.m_params[2] = 1908.238f;
				// t.m_params[3] = 2501.7f;
				t.m_params[0] = 2905.88f;
				t.m_params[1] = 2905.88f;
				t.m_params[2] = 1416.0f;
				t.m_params[3] = 1064.7f;
				cameraInfos.push_back(t);
			}

			for (size_t j = 1; j < subImageBag.size(); ++j)
			{
				CameraInfo t;
				// init images size is 2, subImageBag[0] is initImage[1]
				t.m_id = j + initImages.size() - 1;
				t.m_type = new char[sizeof("PINHOLE")];
				strcpy(t.m_type, "PINHOLE");

				t.m_paramCount = 4;
				t.m_width = subImageBag[0].m_image.cols;
				t.m_height = subImageBag[0].m_image.rows;

				t.m_params = new double[t.m_paramCount];
				t.m_params[0] = 5465.423f;
				t.m_params[1] = 5494.0f;
				t.m_params[2] = 1908.238f;
				t.m_params[3] = 2501.7f;
				cameraInfos.push_back(t);
			}
			std::cout << "CameraInfo completed." << std::endl;
			// ImageInfo
			for (size_t i = 0; i < initImages.size() - 1; i++)
			{
				ImageData t;
				std::vector<double> quat(Matrix2Quaternion(initImages[i].m_R));
				for (int j = 0; j < 4; j++)
				{
					t.m_quaternion[j] = quat[j];
				}
				for (int j = 0; j < 3; j++)
				{
					t.m_translation[j] = initImages[i].m_t.at<double>(j, 0);
				}

				t.m_id = initImages[i].m_Index;
				t.m_cameraId = 0;
				t.m_point2dCount = initImages[i].m_keyPoints.size();
				t.m_name = new char[sizeof(initImages[i].m_imageName) + 1];
				strcpy(t.m_name, initImages[i].m_imageName.c_str());

				t.m_points2d = new Point2dInfo[t.m_point2dCount];
				for (size_t j = 0; j < t.m_point2dCount; ++j)
				{
					t.m_points2d[j].m_x = initImages[i].m_keyPoints[j].pt.x;
					t.m_points2d[j].m_y = initImages[i].m_keyPoints[j].pt.y;
					// if -1 this point is not found in 3D points
					t.m_points2d[j].m_p3dId = initImages[i].m_correspondStructIdx[j];
				}
				t.m_imageData = new uint8_t[initImages[i].m_imageDataSize];
				for (int k = 0; k < initImages[i].m_imageDataSize; ++k)
				{
					t.m_imageData[k] = initImages[i].m_imageData[k];
				}
				t.m_imageDataSize = subImageBag[i].m_imageDataSize;
				imageDatas.push_back(t);
			}

			for (size_t i = 0; i < subImageBag.size(); i++)
			{
				ImageData t;
				std::vector<double> quat(Matrix2Quaternion(subImageBag[i].m_R));
				for (int j = 0; j < 4; j++)
				{
					t.m_quaternion[j] = quat[j];
				}
				for (int j = 0; j < 3; j++)
				{
					t.m_translation[j] = subImageBag[i].m_t.at<double>(j, 0);
				}

				t.m_id = subImageBag[i].m_Index;
				t.m_cameraId = 0;
				t.m_point2dCount = subImageBag[i].m_keyPoints.size();
				t.m_name = new char[sizeof(subImageBag[i].m_imageName) + 1];
				strcpy(t.m_name, subImageBag[i].m_imageName.c_str());

				t.m_points2d = new Point2dInfo[t.m_point2dCount];
				for (size_t j = 0; j < t.m_point2dCount; ++j)
				{
					t.m_points2d[j].m_x = subImageBag[i].m_keyPoints[j].pt.x;
					t.m_points2d[j].m_y = subImageBag[i].m_keyPoints[j].pt.y;
					// if -1 this point is not found in 3D points
					t.m_points2d[j].m_p3dId = subImageBag[i].m_correspondStructIdx[j];
				}
				t.m_imageData = new uint8_t[subImageBag[i].m_imageDataSize];
				for (int k = 0; k < subImageBag[i].m_imageDataSize; ++k)
				{
					t.m_imageData[k] = subImageBag[i].m_imageData[k];
				}
				t.m_imageDataSize = subImageBag[i].m_imageDataSize;
				imageDatas.push_back(t);
			}
			std::cout << "ImageInfo completed." << std::endl;
			// Point3dInfo
			// 1. caculate reprojection error
			cv::Mat FC1K;
			K.convertTo(FC1K, CV_32FC1);
			for (size_t i = 0; i < allReconstructedPoints.size(); ++i)
			{
				float error = 0.0f;
				std::vector<std::pair<int, int>> track;
				for (size_t j = 0; j < initImages.size(); ++j)
				{
					for (size_t t = 0; t < initImages[j].m_correspondStructIdx.size(); t++)
					{
						if (initImages[j].m_correspondStructIdx[t] == i)
						{
							// point found
							std::vector<cv::Point3f> objPoints;
							objPoints.push_back(allReconstructedPoints[i]);
							cv::Mat rvec(3, 1, CV_32FC1);
							cv::Rodrigues(initImages[j].m_R, rvec);
							rvec.convertTo(rvec, CV_32FC1);

							cv::Mat tvec;
							initImages[j].m_t.convertTo(tvec, CV_32FC1);

							std::vector<cv::Point2f> imgPoints;

							cv::projectPoints(objPoints, rvec, initImages[j].m_t, FC1K, cv::noArray(), imgPoints);

							error = cv::norm(imgPoints[0] - initImages[j].m_keyPoints[t].pt);
							std::pair<int, int> p(initImages[j].m_Index, t);
							track.push_back(p);
							break;
						}
					}
				}

				for (size_t j = 0; j < subImageBag.size(); ++j)
				{
					for (size_t t = 0; t < subImageBag[j].m_correspondStructIdx.size(); t++)
					{
						if (subImageBag[j].m_correspondStructIdx[t] == i)
						{
							// point found
							std::vector<cv::Point3f> objPoints;
							objPoints.push_back(allReconstructedPoints[i]);
							cv::Mat rvec(3, 1, CV_32FC1);
							cv::Rodrigues(subImageBag[j].m_R, rvec);
							rvec.convertTo(rvec, CV_32FC1);

							cv::Mat tvec;
							subImageBag[j].m_t.convertTo(tvec, CV_32FC1);

							std::vector<cv::Point2f> imgPoints;
							cv::projectPoints(objPoints, rvec, subImageBag[j].m_t, FC1K, cv::noArray(), imgPoints);

							error = cv::norm(imgPoints[0] - subImageBag[j].m_keyPoints[t].pt);
							std::pair<int, int> p(subImageBag[j].m_Index, t);
							bool pointIsExsitedInInitImage1 = false;
							for (const auto& pairExsited : track)
							{
								if (pairExsited.first == p.first && pairExsited.second == p.second)
								{
									pointIsExsitedInInitImage1 = true;
								}
							}
							if (pointIsExsitedInInitImage1)
							{
								break;
							}
							track.push_back(p);
							break;
						}
					}
				}
				// After get error and track
				// std::cout << "Get errors" << i << std::endl;
				// std::cout << "objPoints: " << allReconstructedPoints.size() << std::endl;

				if (track.size() == 0)
				{
					std::cout << "No track!" << i << std::endl;
				}

				Point3dInfo t;
				t.m_x = allReconstructedPoints[i].x;
				t.m_y = allReconstructedPoints[i].y;
				t.m_z = allReconstructedPoints[i].z;
				t.m_reprojectionError = error;
				t.m_id = i;
				t.m_trackCount = track.size();

				cv::Vec3b color = allPointsColors[i];
				t.m_colorR = static_cast<uint8_t>(color[0]);
				t.m_colorG = static_cast<uint8_t>(color[1]);
				t.m_colorB = static_cast<uint8_t>(color[2]);

				t.m_tracks = new TrackInfo[track.size()];
				for (int j = 0; j < track.size(); j++)
				{
					t.m_tracks[j].m_imageId = track[j].first;
					t.m_tracks[j].m_point2dIdx = track[j].second;
				}

				point3ds.push_back(t);

			}
			// merge initImageBag[1] and subImagesBag[0]
			for (size_t i = 0; i < initImages[1].m_correspondStructIdx.size(); ++i)
			{
				if ((initImages[1].m_correspondStructIdx[i] == -1 && subImageBag[0].m_correspondStructIdx[i] != -1) ||
					(initImages[1].m_correspondStructIdx[i] != -1 && subImageBag[0].m_correspondStructIdx[i] == -1))
				{
					initImages[1].m_correspondStructIdx[i] = std::max(initImages[1].m_correspondStructIdx[i],
						subImageBag[0].m_correspondStructIdx[i]);
					subImageBag[0].m_correspondStructIdx[i] = initImages[1].m_correspondStructIdx[i];
				}
			}
		}
	}
}
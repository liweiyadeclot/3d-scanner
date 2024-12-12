#ifndef TURNTABLESCANNER_H
#define TURNTABLESCANNER_H

#include <memory>
#include "opencv2/opencv.hpp"


class TurntableScanner
{
public:
	TurntableScanner();
	~TurntableScanner();
	int32_t capture();
	int32_t getImgCount();
	cv::Mat nextPicture();
private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

#endif // !TURNTABLESCANNER_H
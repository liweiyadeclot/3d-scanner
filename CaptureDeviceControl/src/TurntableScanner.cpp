#include "TurntableScanner.h"
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

#include "Client.h"
#ifdef _WIN32
#include <windows.h>
#endif

using ssp::Client;
using ssp::ProtocolData;

const uint64_t CAPTURE_DELAY_MS{ 250 };

class TurntableScanner::Impl
{
public:
	Client m_client;
	std::recursive_mutex m_imCountMutex;
	uint32_t m_imCount;
	std::queue<cv::Mat> m_imgQue;
	std::thread m_recvThread;
	static void recvAndHandleData(Client* client, std::queue<cv::Mat>* imgQue, uint32_t* pimCount, std::recursive_mutex* pimCountMutex)
	{
		while (true)
		{
			if (client->getServerCount() < 1)
			{
				client->freshServerList();
				continue;
			}
			ProtocolData recvData{ client->recvFrom(0) };
			if (recvData.getInfo() == "imcount")
			{
				uint32_t appendingImgCount{ 0 };
				recvData.getExtraData(reinterpret_cast<uint8_t*>(&appendingImgCount), sizeof(uint32_t));
				pimCountMutex->lock();
				*pimCount += appendingImgCount;
				pimCountMutex->unlock();
			}
			else if (recvData.getInfo() == "img")
			{
				uint8_t* imgBuf{ new uint8_t[recvData.getExtraDataSize()] };
				recvData.getExtraData(imgBuf, recvData.getExtraDataSize());
				cv::Mat decodedImg{};
				cv::imdecode(cv::Mat(1, recvData.getExtraDataSize(), CV_8UC1, imgBuf), cv::IMREAD_COLOR, &decodedImg);
				imgQue->push(decodedImg);
			}
		}
	}

	Impl() : m_client(), m_imCount(0), m_imgQue(), m_recvThread()
	{
		while (m_client.getServerCount() < 1)
		{
			m_client.freshServerList();
		}
		m_recvThread = std::thread(recvAndHandleData, &m_client, &m_imgQue, &m_imCount, &m_imCountMutex);
	}

	~Impl()
	{
		m_recvThread.detach();
	}

	int32_t capture()
	{
		if (m_client.getServerCount() < 1)
		{
			m_client.freshServerList();
			return -1;
		}
		m_client.sendTo(0, ProtocolData{ "capture", reinterpret_cast<const uint8_t*>(&CAPTURE_DELAY_MS), sizeof(uint64_t) });
		return 0;
	}

	int32_t getImgCount()
	{
		m_imCountMutex.lock();
		m_imCountMutex.unlock();
		return m_imCount;
	}

	cv::Mat nextPicture()
	{
		if (m_imCount == 0)
		{
			return cv::Mat{};
		}
		while (m_imgQue.empty())
		{
			;
		}
		cv::Mat img{ std::move(m_imgQue.front()) };
		m_imgQue.pop();
		m_imCountMutex.lock();
		--m_imCount;
		m_imCountMutex.unlock();
		return img;
	}
};

TurntableScanner::TurntableScanner() : m_impl{ std::make_unique<Impl>() }
{
}

TurntableScanner::~TurntableScanner()
{
}

int32_t TurntableScanner::capture()
{
	return m_impl->capture();
}

int32_t TurntableScanner::getImgCount()
{
	return m_impl->getImgCount();
}

cv::Mat TurntableScanner::nextPicture()
{
	return m_impl->nextPicture();
}
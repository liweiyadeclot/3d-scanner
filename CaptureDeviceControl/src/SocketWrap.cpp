#include "SocketWrap.h"

namespace ssp {

	SOCKET MakeUDPSocket(const char* selfIp, const char* targetIp, u_short port)
	{

		WSADATA wsaData;
		SOCKET ret{ INVALID_SOCKET };
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			perror("WSAStartup failed");
			return ret;
		}

		ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (INVALID_SOCKET == ret)
		{
			perror("udp socket failed");
			WSACleanup();
			return ret;
		}

		bool broadcastEnable = true;
		if (-1 == setsockopt(ret, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcastEnable, sizeof(broadcastEnable))) {
			perror("udp setsockopt failed");
			WSACleanup();
			closesocket(ret);
			return INVALID_SOCKET;
		}

		sockaddr_in broadCastAddr{ 0 };
		broadCastAddr.sin_family = AF_INET;
		broadCastAddr.sin_port = htons(port);
		inet_pton(AF_INET, selfIp, &broadCastAddr.sin_addr.s_addr);
		if (SOCKET_ERROR == bind(ret, (struct sockaddr*)&broadCastAddr, sizeof(broadCastAddr)))
		{
			perror("udp bind failed");
			closesocket(ret);
			return INVALID_SOCKET;
		}

		return ret;
	}

	int32_t Select(SOCKET s, uint32_t timeout_us)
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(s, &readfds);
		int32_t select_result{ -1 };
		if (timeout_us == -1)
		{
			select_result = select(0, &readfds, nullptr, nullptr, nullptr);
		}
		else
		{
			timeval timeout{ timeout_us / 1000000, timeout_us % 1000000 };
			select_result = select(0, &readfds, nullptr, nullptr, &timeout);
		}
		if (select_result == SOCKET_ERROR)
		{
			return -1;
		}
		else if (select_result == 0)
		{
			return 0;
		}
		else if (FD_ISSET(s, &readfds))
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	ProtocolData RecvFromTCP(SOCKET s)
	{
		uint32_t dataLen{ 0 };
		int len = recv(s, (char*)&dataLen, sizeof(dataLen), 0);
		if (len == -1)
		{
			perror("recv fail");
			return ProtocolData();
		}
		char* buf{ new char[dataLen + 4] };
		*(uint32_t*)buf = dataLen;
		char* p = buf + 4;
		dataLen = dataLen - (len - 4);
		while (dataLen > 0)
		{
			len = recv(s, p, dataLen, 0);
			if (len <= 0)
			{
				break;
			}
			p += len;
			dataLen -= len;
		}
		ProtocolData ret{ ProtocolData::fromRawData((const uint8_t*)buf) };
		delete buf;
		return ret;
	}

	ProtocolData RecvFromUDP(SOCKET s, sockaddr_in& addr, uint32_t bufSize)
	{
		if (s == -1)
		{
			return ProtocolData();
		}
		socklen_t addrLen{ sizeof(sockaddr_in) };
		uint32_t dataLen{ 0 };
		char* buf{ new char[bufSize] };
		int len = recvfrom(s, buf, bufSize, 0, (sockaddr*)&addr, &addrLen);
		dataLen = *(uint32_t*)buf + 4 - len;
		char* p = buf + 4;
		while (dataLen > 0)
		{
			len = recvfrom(s, p, dataLen, 0, (sockaddr*)&addr, &addrLen);
			if (len <= 0)
			{
				break;
			}
			p += len;
			dataLen -= len;
		}
		ProtocolData ret{ ProtocolData::fromRawData((const uint8_t*)buf) };
		delete buf;
		return ret;
	}

	bool SendTo(SOCKET s, const ProtocolData& data)
	{
		uint32_t rawDataLen{ data.getRawDataSize() };
		uint8_t* rawData{ new uint8_t[rawDataLen] };
		data.toRawData(rawData, rawDataLen);
		int sendLen{ send(s, (const char*)rawData, rawDataLen, 0) };
		delete rawData;
		return sendLen == rawDataLen;
	}

	bool SendTo(SOCKET s, sockaddr_in& addr, const ProtocolData& data)
	{
		uint32_t rawDataLen{ data.getRawDataSize() };
		uint8_t* rawData{ new uint8_t[rawDataLen] };
		data.toRawData(rawData, rawDataLen);
		int sendLen{ sendto(s, (const char*)rawData, rawDataLen, 0, (sockaddr*)&addr, sizeof(sockaddr_in)) };
		delete rawData;
		return sendLen == rawDataLen;
	}

	bool IsTCPBroken(SOCKET s)
	{
		ProtocolData data{ ProtocolData("t", nullptr, 0) };
		char buf[sizeof(ssp::rawDataSize_t) + 2]{0};
		data.toRawData((uint8_t*)buf, sizeof(buf));
		if (SOCKET_ERROR != send(s, buf, sizeof(buf), 0))
		{
			if ((recv(s, buf, sizeof(buf), 0) == sizeof(buf)) && ((*(ssp::rawDataSize_t*)buf) == 2))
			{
				return false;
			}
		}
		return true;
	}
}
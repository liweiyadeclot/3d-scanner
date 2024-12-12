#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
#include "ProtocolData.h"

namespace ssp {
	SOCKET MakeUDPSocket(const char* selfIp, const char* targetIp, u_short port);

	ProtocolData RecvFromTCP(SOCKET s);

	ProtocolData RecvFromUDP(SOCKET s, sockaddr_in& addr, uint32_t bufSize=1024);

	bool SendTo(SOCKET s, const ssp::ProtocolData& data);

	bool SendTo(SOCKET s, sockaddr_in& addr, const ProtocolData& data);

	bool IsTCPBroken(SOCKET s);
}
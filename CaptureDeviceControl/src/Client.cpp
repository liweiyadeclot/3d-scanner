#include "Client.h"
#include <vector>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
#include <thread>
#include <mutex>

#include "SocketWrap.h"

using namespace ssp;

const char SELF_IP[]{ "0.0.0.0" };
const char BROADCAST_IP[]{ "192.168.137.255" };
const u_short BROADCAST_PORT{ 0x3113U ^ 0x5181U ^ 0x2181U ^ 0x5143U + 0x1192U ^ 0x0U };//"camera broadcast"
const u_short TCP_PORT{ 0x3113U ^ 0x5181U ^ 0x2031U ^ 0x6U };//"camera tcp"

class Client::ClientImpl
{
public:
	static void recvAndFreshServer(Client::ClientImpl* client)
	{
		while (client->m_broadcastSocket != INVALID_SOCKET)
		{
			sockaddr_in addr{ 0 };
			if (1 != Select(client->m_broadcastSocket, 1000000))
			{
				continue;
			}
			ProtocolData data{ RecvFromUDP(client->m_broadcastSocket, addr) };
			if (data.getInfo() != "ssp_im_server")
			{
				continue;
			}
			bool isExist{ false };
			for (sockaddr_in& a : client->m_serverAddrs)
			{
				if (a.sin_addr.s_addr == addr.sin_addr.s_addr)
				{
					isExist = true;
					break;
				}
			}
			if (!isExist)
			{
				SOCKET s{ socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) };
				if (INVALID_SOCKET == s)
				{
					perror("tcp socket failed");
					continue;
				}
				addr.sin_port = htons(TCP_PORT);
				if (SOCKET_ERROR == connect(s, (sockaddr*)&addr, sizeof(sockaddr_in)))
				{
					perror("tcp connect failed");
					closesocket(s);
					continue;
				}
				client->m_ServerListWriteLock->lock();
				client->m_serverSockets.push_back(s);
				client->m_serverAddrs.push_back(addr);
				client->m_ServerListWriteLock->unlock();
			}
		}
	}

	ClientImpl()
		:m_serverSockets(),
		m_serverAddrs(),
		m_broadcastSocket(INVALID_SOCKET),
		m_serverListFreshThread(),
		m_ServerListWriteLock(new std::recursive_mutex())
	{
		m_broadcastSocket = MakeUDPSocket(SELF_IP, BROADCAST_IP, BROADCAST_PORT);
		if (INVALID_SOCKET == m_broadcastSocket)
		{
			perror("MakeUDPSocket failed");
			return;
		}
		m_serverListFreshThread = std::thread(recvAndFreshServer, this);
		freshServerList();
	}

	~ClientImpl()
	{
		m_ServerListWriteLock->lock();
		for (SOCKET s : m_serverSockets)
		{
			closesocket(s);
		}
		SOCKET s{ m_broadcastSocket };
		m_broadcastSocket = INVALID_SOCKET;
		m_ServerListWriteLock->unlock();
		m_serverListFreshThread.join();
		closesocket(s);
		WSACleanup();
		delete m_ServerListWriteLock;
	}

	void freshServerList()
	{
		m_ServerListWriteLock->lock();
		for (int i = 0; i < m_serverSockets.size(); i++)
		{
			SOCKET s{ m_serverSockets[i] };
			if (IsTCPBroken(s))
			{
				closesocket(s);
				m_serverSockets.erase(m_serverSockets.begin() + i);
				m_serverAddrs.erase(m_serverAddrs.begin() + i);
			}
		}
		m_ServerListWriteLock->unlock();
		ProtocolData data{ ProtocolData("ssp_find_server", nullptr, 0) };

		sockaddr_in broadCastAddr{ 0 };
		broadCastAddr.sin_family = AF_INET;
		broadCastAddr.sin_port = htons(BROADCAST_PORT);
		inet_pton(AF_INET, BROADCAST_IP, &broadCastAddr.sin_addr.s_addr);
		SendTo(m_broadcastSocket, broadCastAddr, data);
	}

	uint32_t getServerCount() const
	{
		m_ServerListWriteLock->lock();
		m_ServerListWriteLock->unlock();
		return (uint32_t)m_serverSockets.size();
	}

	ProtocolData recvFrom(uint32_t index, uint32_t timeout_us) const
	{
		m_ServerListWriteLock->lock();
		m_ServerListWriteLock->unlock();
		ProtocolData data{ ProtocolData() };
		if (index >= getServerCount())
		{
			return data;
		}
		if (1 != Select(m_serverSockets[index], timeout_us))
		{
			return data;
		}
		data = RecvFromTCP(m_serverSockets[index]);
		return data;
	}

	bool sendTo(uint32_t index, const ProtocolData& data) const
	{
		m_ServerListWriteLock->lock();
		m_ServerListWriteLock->unlock();
		if (index >= getServerCount())
		{
			return false;
		}
		return SendTo(m_serverSockets[index], data);
	}

	std::vector<SOCKET> m_serverSockets;
	std::vector<sockaddr_in> m_serverAddrs;
	SOCKET m_broadcastSocket;
	std::thread m_serverListFreshThread;
	std::recursive_mutex* m_ServerListWriteLock;
};

Client::Client()
	: m_impl(new ClientImpl())
{

}

Client::~Client()
{
	delete m_impl;
}

void Client::freshServerList()
{
	m_impl->freshServerList();
}

uint32_t Client::getServerCount() const
{
	return m_impl->getServerCount();
}

ProtocolData Client::recvFrom(uint32_t index, uint32_t timeout_us) const
{
	return m_impl->recvFrom(index, timeout_us);
}

bool Client::sendTo(uint32_t index, const ProtocolData& data) const
{
	return m_impl->sendTo(index, data);
}

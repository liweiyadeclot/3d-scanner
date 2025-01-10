#pragma once
#include "ProtocolData.h"

namespace ssp {
	class Client {
	public:
		Client();
		~Client();
		//注意，刷新服务器列表的实现中会向服务器发包，并检测返回的包的内容
		void freshServerList();
		uint32_t getServerCount() const;
		ProtocolData recvFrom(uint32_t index, uint32_t timeout_us = -1) const;
		bool sendTo(uint32_t index, const ProtocolData& data) const;
	private:
		class ClientImpl;
		ClientImpl* m_impl;
	};
}
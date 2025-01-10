#pragma once
#include "ProtocolData.h"

namespace ssp {
	class Client {
	public:
		Client();
		~Client();
		//ע�⣬ˢ�·������б��ʵ���л������������������ⷵ�صİ�������
		void freshServerList();
		uint32_t getServerCount() const;
		ProtocolData recvFrom(uint32_t index, uint32_t timeout_us = -1) const;
		bool sendTo(uint32_t index, const ProtocolData& data) const;
	private:
		class ClientImpl;
		ClientImpl* m_impl;
	};
}
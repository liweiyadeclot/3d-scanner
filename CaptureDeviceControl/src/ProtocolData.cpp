#include "ProtocolData.h"
#include <cstring>
#include <iostream>

using namespace ssp;

ProtocolData::ProtocolData()
	: m_info(),
	m_extraData(nullptr),
	m_extraDataLen(0)
{
}

ProtocolData::ProtocolData(const std::string& info, const uint8_t* extraData, rawDataSize_t extraDataLen)
	: m_info(info),
	m_extraData(nullptr),
	m_extraDataLen(extraDataLen)
{
	if (extraData != nullptr && extraDataLen > 0)
	{
		m_extraData = new uint8_t[extraDataLen];
		memcpy(m_extraData, extraData, extraDataLen);
	}
}

ProtocolData::ProtocolData(const std::string&& info, const uint8_t* extraData, rawDataSize_t extraDataLen)
	: m_info(info),
	m_extraData(nullptr),
	m_extraDataLen(extraDataLen)
{
	if (extraData != nullptr && extraDataLen > 0)
	{
		m_extraData = new uint8_t[extraDataLen];
		memcpy(m_extraData, extraData, extraDataLen);
	}
}

ProtocolData::~ProtocolData()
{
	if (m_extraData != nullptr)
	{
		delete[] m_extraData;
		m_extraData = nullptr;
	}
}

rawDataSize_t ProtocolData::getExtraData(uint8_t* data, rawDataSize_t bufLen) const
{
	if (m_extraDataLen > 0 && bufLen >= m_extraDataLen)
	{
		memcpy(data, m_extraData, m_extraDataLen);
		return m_extraDataLen;
	}
	return 0;
}

std::string ProtocolData::getInfo() const
{
	return m_info;
}

bool ProtocolData::setInfo(const std::string& info)
{
	m_info = info;
	return true;
}

void ProtocolData::setExtraData(const uint8_t* data, rawDataSize_t len)
{
	uint8_t* newExtraData = nullptr;
	if (len > 0)
	{
		newExtraData = new uint8_t[len];
		memcpy(newExtraData, data, len);
	}
	if (m_extraData != nullptr)
	{
		delete[] m_extraData;
	}
	m_extraData = newExtraData;
	m_extraDataLen = len;
}

rawDataSize_t ProtocolData::getExtraDataSize() const
{
	return m_extraDataLen;
}

rawDataSize_t ProtocolData::getRawDataSize() const
{
	return (rawDataSize_t)sizeof(rawDataSize_t) + (rawDataSize_t)m_info.size() + 1 + m_extraDataLen;
}

rawDataSize_t ProtocolData::toRawData(uint8_t* buffer, rawDataSize_t bufLen) const
{
	rawDataSize_t len{ getRawDataSize() };
	if (len > bufLen)
	{
		return 0;
	}
	*((rawDataSize_t*)buffer) = len - sizeof(rawDataSize_t);
	uint8_t* p = buffer + sizeof(rawDataSize_t);
	memcpy(p, m_info.c_str(), m_info.size());
	p[m_info.size()] = ' ';
	if (m_extraDataLen > 0)
	{
		memcpy(p + m_info.size() + 1, m_extraData, m_extraDataLen);
	}
	return len;
}

ProtocolData ProtocolData::fromRawData(const uint8_t* data)
{
	rawDataSize_t len = *((rawDataSize_t*)data);
	data = data + sizeof(rawDataSize_t);
	std::string info{};
	rawDataSize_t i = 0;
	for (; i < len; ++i)
	{
		if (data[i] == ' ')
		{
			break;
		}
		info.push_back(data[i]);
	}
	rawDataSize_t extraDataLen{ 0 };
	const uint8_t* extraData{ nullptr };
	if (len > i + 1)
	{
		extraDataLen = len - i - 1;
		extraData = data + i + 1;
	}
	return ProtocolData(info, extraData, extraDataLen);
}

ProtocolData::ProtocolData(const ProtocolData& data)
	: m_info(),
	m_extraData(nullptr),
	m_extraDataLen(0)
{
	*this = data;
}

ProtocolData::ProtocolData(const ProtocolData&& data) noexcept
	: m_info(),
	m_extraData(nullptr),
	m_extraDataLen(0)
{
	*this = data;
}

ProtocolData& ProtocolData::operator=(const ProtocolData& data)
{
	if (this != &data)
	{
		m_info = data.m_info;
		uint8_t* newExtraData = nullptr;
		if (data.m_extraData != nullptr && data.m_extraDataLen > 0)
		{
			newExtraData = new uint8_t[data.m_extraDataLen];
			memcpy(newExtraData, data.m_extraData, data.m_extraDataLen);
		}
		if (m_extraData != nullptr)
		{
			delete[] m_extraData;
		}
		m_extraData = newExtraData;
		m_extraDataLen = data.m_extraDataLen;
	}
	return *this;
}

ProtocolData& ProtocolData::operator=(ProtocolData&& data) noexcept
{
	if (this != &data)
	{
		m_info = std::move(data.m_info);

		if (m_extraData != nullptr)
		{
			delete[] m_extraData;
		}

		m_extraData = data.m_extraData;
		m_extraDataLen = data.m_extraDataLen;

		data.m_extraData = nullptr;
		data.m_extraDataLen = 0;
	}
	return *this;
}
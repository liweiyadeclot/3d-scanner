#pragma once

#include <string>

namespace ssp
{
    using rawDataSize_t = uint32_t;
    class ProtocolData
    {
    public:
        ProtocolData();
        ProtocolData(const std::string& info, const uint8_t* extraData, rawDataSize_t extraDataLen);
        ProtocolData(const std::string&& info, const uint8_t* extraData, rawDataSize_t extraDataLen);
        ~ProtocolData();
        ProtocolData(const ProtocolData& data);
        ProtocolData(const ProtocolData&& data) noexcept;
        ProtocolData& operator=(const ProtocolData& data);
        ProtocolData& operator=(ProtocolData&& data) noexcept;
        bool setInfo(const std::string& info);
        void setExtraData(const uint8_t* data, rawDataSize_t len);
        std::string getInfo() const;
        rawDataSize_t getExtraData(uint8_t* data, rawDataSize_t bufLen) const;
        rawDataSize_t getExtraDataSize() const;
        rawDataSize_t getRawDataSize() const;
        rawDataSize_t toRawData(uint8_t* buffer, rawDataSize_t bufLen) const;
        static ProtocolData fromRawData(const uint8_t* data);
    private:
        std::string m_info;
        uint8_t* m_extraData;
        rawDataSize_t m_extraDataLen;
    };
}
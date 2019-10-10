//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <armnn/Exceptions.hpp>

#include <memory>

namespace armnn
{

namespace profiling
{

class Packet
{
public:
    Packet()
        : m_Header(0)
        , m_Length(0)
        , m_Data(nullptr)
    {}

    Packet(uint32_t header)
        : m_Header(header)
        , m_Length(0)
        , m_Data(nullptr)
    {
        m_PacketId = ((header >> 16) & 1023);
        m_PacketFamily = (header >> 26);
    }

    Packet(uint32_t header, uint32_t length, std::unique_ptr<unsigned char[]>& data)
        : m_Header(header)
        , m_Length(length)
        , m_Data(std::move(data))
    {
        m_PacketId = ((header >> 16) & 1023);
        m_PacketFamily = (header >> 26);

        if (length == 0 && m_Data != nullptr)
        {
            throw armnn::InvalidArgumentException("Data should be null when length is zero");
        }
    }

    Packet(Packet&& other)
        : m_Header(other.m_Header)
        , m_PacketFamily(other.m_PacketFamily)
        , m_PacketId(other.m_PacketId)
        , m_Length(other.m_Length)
        , m_Data(std::move(other.m_Data))
    {
        other.m_Header = 0;
        other.m_PacketFamily = 0;
        other.m_PacketId = 0;
        other.m_Length = 0;
    }

    ~Packet() = default;

    Packet(const Packet& other) = delete;
    Packet& operator=(const Packet&) = delete;
    Packet& operator=(Packet&&) = default;

    uint32_t GetHeader() const                 { return m_Header;        }
    uint32_t GetPacketFamily() const           { return m_PacketFamily;  }
    uint32_t GetPacketId() const               { return m_PacketId;      }
    uint32_t GetPacketClass() const            { return m_PacketId >> 3; }
    uint32_t GetPacketType() const             { return m_PacketId & 7;  }
    uint32_t GetLength() const                 { return m_Length;        }
    const unsigned char* const GetData() const { return m_Data.get();    }

    bool IsEmpty() { return m_Header == 0 && m_Length == 0; }

private:
    uint32_t m_Header;
    uint32_t m_PacketFamily;
    uint32_t m_PacketId;
    uint32_t m_Length;
    std::unique_ptr<unsigned char[]> m_Data;
};

} // namespace profiling

} // namespace armnn

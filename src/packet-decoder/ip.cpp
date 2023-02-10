#include "packet-decoder/ip.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <utility>

#include <arpa/inet.h>

#include "packet-decoder/base.hpp"
#include "utils.hpp"

namespace {

constexpr auto kNextProtocolUdp = 0x11;

#pragma pack(1)
struct Header {
    std::uint8_t version: 4;
    std::uint8_t ihl: 4;
    std::uint8_t serviceType;
    std::uint16_t totalLength;
    std::uint16_t identification;
    std::uint8_t flags: 3;
    std::uint16_t fragmentOffset: 13;
    std::uint8_t ttl;
    std::uint8_t protocol;
    std::uint16_t headerChecksum;
    std::uint32_t addressSource;
    std::uint32_t addressDestination;
};
static_assert(sizeof(Header) == 20U);
#pragma pack(0)

}

namespace Pkt {

IpPacketDecoder::IpPacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : PacketDecoder(std::move(delegator))
{
    assert(not m_packet);
}

auto IpPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    assert(m_packet);
    assert(m_packet->size() > sizeof(Header));

    [[maybe_unused]] auto header = mapDataTo<const Header>(m_packet->data());
    assert(::ntohs(header->totalLength) == m_packet->size());
    assert(header->protocol == kNextProtocolUdp);

    auto payload = Payload{(m_packet->cbegin() + sizeof(Header)), m_packet->cend()};
    assert(payload.size() == (m_packet->size() - sizeof(Header)));

    return payload;
}

}

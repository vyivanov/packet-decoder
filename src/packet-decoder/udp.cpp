#include "packet-decoder/udp.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <utility>

#include <arpa/inet.h>

#include "packet-decoder/base.hpp"
#include "utils.hpp"

namespace {

#pragma pack(1)
struct Header {
    std::uint16_t portSource;
    std::uint16_t portDestination;
    std::uint16_t totalLength;
    std::uint16_t checksum;
};
static_assert(sizeof(Header) == 8U);
#pragma pack(0)

}

namespace Pkt {

UdpPacketDecoder::UdpPacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : PacketDecoder(std::move(delegator))
{
    assert(not m_packet);
}

auto UdpPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    assert(m_packet);
    assert(m_packet->size() > sizeof(Header));

    [[maybe_unused]] auto header = mapDataTo<const Header>(m_packet->data());
    assert(::ntohs(header->totalLength) == m_packet->size());

    auto payload = Payload{(m_packet->cbegin() + sizeof(Header)), m_packet->cend()};
    assert(payload.size() == (m_packet->size() - sizeof(Header)));

    return payload;
}

}

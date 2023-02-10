#include "packet-decoder/ethernet.hpp"

#include <cassert>
#include <optional>
#include <utility>

#include <arpa/inet.h>

#include "packet-decoder/base.hpp"
#include "utils.hpp"

namespace {

constexpr auto kEthernetTypeIPv4 = 0x08'00;

#pragma pack(1)
struct Header {
    std::uint64_t macAddressDestination: 48;
    std::uint64_t macAddressSource: 48;
    std::uint16_t ethernetType;
};
static_assert(sizeof(Header) == 14U);
#pragma pack(0)

}

namespace Pkt {

EthernetPacketDecoder::EthernetPacketDecoder(Packet data)
    : PacketDecoder(std::move(data))
{
    assert(m_packet);
}

auto EthernetPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    assert(m_packet->size() > sizeof(Header));

    [[maybe_unused]] auto header = mapDataTo<const Header>(m_packet->data());
    assert(::ntohs(header->ethernetType) == kEthernetTypeIPv4);

    auto payload = Payload{(m_packet->cbegin() + sizeof(Header)), m_packet->cend()};
    assert(payload.size() == (m_packet->size() - sizeof(Header)));
    assert((46U <= payload.size()) and (payload.size() <= 1'500U));

    return payload;
}

}

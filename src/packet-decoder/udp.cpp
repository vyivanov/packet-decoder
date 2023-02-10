#include "packet-decoder/udp.hpp"

#include <memory>
#include <optional>
#include <utility>

#include "packet-decoder/base.hpp"

namespace Pkt {

UdpPacketDecoder::UdpPacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : PacketDecoder(std::move(delegator))
{

}

auto UdpPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    return std::nullopt;
}

}

#include "packet-decoder/ip.hpp"

#include <memory>
#include <optional>
#include <utility>

#include "packet-decoder/base.hpp"

namespace Pkt {

IpPacketDecoder::IpPacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : PacketDecoder(std::move(delegator))
{

}

auto IpPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    return std::nullopt;
}

}

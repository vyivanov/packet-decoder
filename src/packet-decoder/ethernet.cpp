#include "packet-decoder/ethernet.hpp"

#include <optional>
#include <utility>

#include "packet-decoder/base.hpp"

namespace Pkt {

EthernetPacketDecoder::EthernetPacketDecoder(Packet data)
    : PacketDecoder(std::move(data))
{

}

auto EthernetPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    return std::nullopt;
}

}

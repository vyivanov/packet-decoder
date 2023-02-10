#include "packet-decoder/base.hpp"

#include <memory>
#include <optional>

namespace Pkt {

PacketDecoder::PacketDecoder(std::unique_ptr<PacketDecoder> delegator)
{
    (void) delegator;
}

PacketDecoder::PacketDecoder(Packet data)
{
    (void) data;
}

auto PacketDecoder::payload() -> std::optional<Payload>
{
    return std::nullopt;
}

}

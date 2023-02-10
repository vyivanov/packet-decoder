#include "packet-decoder/simba.hpp"

#include <memory>
#include <optional>
#include <utility>

#include "packet-decoder/base.hpp"

namespace Pkt {

SimbaPacketDecoder::SimbaPacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : PacketDecoder(std::move(delegator))
{

}

auto SimbaPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    return std::nullopt;
}

}

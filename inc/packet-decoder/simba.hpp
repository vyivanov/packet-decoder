#pragma once

#include <memory>
#include <optional>

#include "packet-decoder/base.hpp"

namespace Pkt {

class SimbaPacketDecoder final: public PacketDecoder {
public:
    explicit SimbaPacketDecoder(std::unique_ptr<PacketDecoder> delegator);
private:
    std::optional<Payload> payloadImpl() override;
};

}

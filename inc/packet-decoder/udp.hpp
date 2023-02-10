#pragma once

#include <memory>
#include <optional>

#include "packet-decoder/base.hpp"

namespace Pkt {

class UdpPacketDecoder final: public PacketDecoder {
public:
    explicit UdpPacketDecoder(std::unique_ptr<PacketDecoder> delegator);
private:
    std::optional<Payload> payloadImpl() override;
};

}

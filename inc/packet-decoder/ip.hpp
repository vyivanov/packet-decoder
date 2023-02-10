#pragma once

#include <memory>
#include <optional>

#include "packet-decoder/base.hpp"

namespace Pkt {

class IpPacketDecoder final: public PacketDecoder {
public:
    explicit IpPacketDecoder(std::unique_ptr<PacketDecoder> delegator);
private:
    std::optional<Payload> payloadImpl() override;
};

}

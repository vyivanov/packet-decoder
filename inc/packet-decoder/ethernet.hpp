#pragma once

#include <optional>

#include "packet-decoder/base.hpp"

namespace Pkt {

class EthernetPacketDecoder final: public PacketDecoder {
public:
    explicit EthernetPacketDecoder(Packet data);
private:
    std::optional<Payload> payloadImpl() override;
};

}

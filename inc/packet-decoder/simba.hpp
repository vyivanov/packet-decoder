#pragma once

#include <memory>
#include <optional>

#include <nlohmann/json.hpp>

#include "packet-decoder/base.hpp"

namespace Pkt {

class SimbaPacketDecoder final: public PacketDecoder {
public:
    explicit SimbaPacketDecoder(std::unique_ptr<PacketDecoder> delegator);
    nlohmann::ordered_json decode();
private:
    std::optional<Payload> payloadImpl() override;
};

}

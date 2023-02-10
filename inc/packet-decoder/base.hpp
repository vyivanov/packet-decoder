#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace Pkt {

class PacketDecoder {
public:
    using Packet = std::vector<std::uint8_t>;
    using Payload = std::vector<std::uint8_t>;
    explicit PacketDecoder(std::unique_ptr<PacketDecoder> delegator);
    explicit PacketDecoder(Packet data);
    std::optional<Payload> payload();
    virtual ~PacketDecoder() noexcept = default;
protected:
    std::unique_ptr<PacketDecoder> m_delegator = {};
    std::optional<Packet> m_packet = {};
private:
    virtual std::optional<Payload> payloadImpl() = 0;
};

}

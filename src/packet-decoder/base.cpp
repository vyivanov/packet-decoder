#include "packet-decoder/base.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <utility>

namespace Pkt {

PacketDecoder::PacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : m_delegator(std::move(delegator))
{
    assert(m_delegator);
    assert(not m_packet);
}

PacketDecoder::PacketDecoder(Packet data)
    : m_packet(std::move(data))
{
    assert(not m_delegator);
    assert(m_packet);
}

auto PacketDecoder::payload() -> std::optional<Payload>
{
    if (not m_packet) {
        assert(m_delegator);
        m_packet = m_delegator->payload();
    }

    assert(m_packet);
    return payloadImpl();
}

}

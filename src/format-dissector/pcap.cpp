#include "format-dissector/pcap.hpp"

#include <memory>
#include <optional>

#include "data-reader/iface.hpp"
#include "format-dissector/iface.hpp"

namespace Pkt {

PcapFormatDissector::PcapFormatDissector(std::unique_ptr<DataReader> reader)
{
    (void) reader;
}

auto PcapFormatDissector::nextPacket() -> std::optional<Packet>
{
    return std::nullopt;
}

}

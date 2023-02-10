#pragma once

#include <memory>
#include <optional>

#include "data-reader/iface.hpp"
#include "format-dissector/iface.hpp"

namespace Pkt {

class PcapFormatDissector final: public FormatDissector {
public:
    explicit PcapFormatDissector(std::unique_ptr<DataReader> reader);
    std::optional<Packet> nextPacket() override;
private:
    std::unique_ptr<DataReader> m_reader = {};
};

}

#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace Pkt {

class FormatDissector {
public:
    using Ptr = std::unique_ptr<FormatDissector>;
    struct Packet {
        using Data = std::vector<std::uint8_t>;
        using Timestamp = std::chrono::utc_time<std::chrono::nanoseconds>;
        Data data;
        Timestamp timestamp;
    };
    virtual std::optional<Packet> nextPacket() = 0;
    virtual ~FormatDissector() noexcept = default;
};

}

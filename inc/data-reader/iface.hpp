#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace Pkt {

class DataReader {
public:
    using Ptr = std::unique_ptr<DataReader>;
    using ChunkData = std::vector<std::uint8_t>;
    using ChunkSize = std::size_t;
    virtual std::optional<ChunkData> nextChunk(ChunkSize length) = 0;
    virtual ~DataReader() noexcept = default;
};

}

#pragma once

#include <filesystem>
#include <optional>

#include "data-reader/iface.hpp"

namespace Pkt {

class LocalFileReader final: public DataReader {
public:
    explicit LocalFileReader(const std::filesystem::path& path);
    std::optional<ChunkData> nextChunk(ChunkSize length) override;
};

}

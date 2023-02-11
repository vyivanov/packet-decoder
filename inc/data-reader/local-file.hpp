#pragma once

#include <filesystem>
#include <fstream>
#include <optional>

#include "data-reader/iface.hpp"

namespace Pkt {

class LocalFileReader final: public DataReader {
public:
    explicit LocalFileReader(const std::filesystem::path& path);
    std::optional<ChunkData> nextChunk(ChunkSize length) override;
private:
    std::ifstream m_file = {};
};

}

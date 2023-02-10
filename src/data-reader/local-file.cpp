#include "data-reader/local-file.hpp"

#include <filesystem>
#include <optional>

#include "data-reader/iface.hpp"

namespace Pkt {

LocalFileReader::LocalFileReader(const std::filesystem::path& path)
{
    (void) path;
}

auto LocalFileReader::nextChunk(const ChunkSize length) -> std::optional<ChunkData>
{
    (void) length;
    return std::nullopt;
}

}

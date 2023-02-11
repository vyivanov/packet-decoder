#include "data-reader/local-file.hpp"

#include <cassert>
#include <filesystem>
#include <ios>
#include <optional>
#include <type_traits>

#include "data-reader/iface.hpp"

namespace Pkt {

LocalFileReader::LocalFileReader(const std::filesystem::path& path)
    : m_file(path, std::ios::binary)
{
    assert(std::filesystem::exists(path));

    assert(m_file.is_open());
    assert(m_file.good());
}

auto LocalFileReader::nextChunk(const ChunkSize length) -> std::optional<ChunkData>
{
    assert(length > 0);
    auto chunk = ChunkData(length, ChunkData::value_type{});
    assert(chunk.size() == length);

    using Data = std::add_pointer_t<decltype(m_file)::char_type>;
    using Size = std::streamsize;

    m_file.read(reinterpret_cast<Data>(chunk.data()), static_cast<Size>(length));

    const auto read = m_file.gcount();
    assert(read <= static_cast<Size>(length));

    if (not read) {
        return std::nullopt;
    }

    chunk.resize(read);
    return chunk;
}

}

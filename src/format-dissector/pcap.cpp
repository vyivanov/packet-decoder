#include "format-dissector/pcap.hpp"

#include <bit>
#include <cassert>
#include <chrono>
#include <memory>
#include <optional>
#include <utility>

#include "data-reader/iface.hpp"
#include "format-dissector/iface.hpp"
#include "utils.hpp"

namespace {

constexpr auto kMagicNumber = 0xA1B2C3D4;
constexpr auto kVersionMajor = 2U;
constexpr auto kVersionMinor = 4U;
constexpr auto kLinkLayerEthernet = 1U;

#pragma pack(1)
struct GlobalHeader {
    std::uint32_t magicNumber;
    std::uint16_t versionMajor;
    std::uint16_t versionMinor;
    std::uint32_t reserved_1;
    std::uint32_t reserved_2;
    std::uint32_t snapshotLength;
    std::uint32_t linkType;
};
static_assert(sizeof(GlobalHeader) == 24U);
struct PacketHeader {
    std::uint32_t timestampSeconds;
    std::uint32_t timestampMicroseconds;
    std::uint32_t packetLengthCaptured;
    std::uint32_t packetLengthOriginal;
};
static_assert(sizeof(PacketHeader) == 16U);
#pragma pack(0)

}

namespace Pkt {

PcapFormatDissector::PcapFormatDissector(std::unique_ptr<DataReader> reader)
    : m_reader(std::move(reader))
{
    assert(m_reader);

    const auto headerData = m_reader->nextChunk(sizeof(GlobalHeader));
    assert(headerData.has_value());
    assert(headerData->size() == sizeof(GlobalHeader));

    static_assert((std::endian::native == std::endian::little), "cover big endian architecture");
    [[maybe_unused]] auto header = mapDataTo<const GlobalHeader>(headerData->data());

    assert(header->magicNumber == kMagicNumber);
    assert(header->versionMajor == kVersionMajor);
    assert(header->versionMinor == kVersionMinor);
    assert(header->reserved_1 == 0U);
    assert(header->reserved_2 == 0U);
    assert(header->snapshotLength > 0U);

    assert((header->linkType bitand (0xFF'FF)) == kLinkLayerEthernet);
    assert((header->linkType bitand (1 << 26)) == 0U);
    assert((header->linkType bitand (1 << 27)) == 0U);
    assert((header->linkType bitand (0b1111 << 28)) == 0U);
}

auto PcapFormatDissector::nextPacket() -> std::optional<Packet>
{
    const auto headerData = m_reader->nextChunk(sizeof(PacketHeader));
    if (not headerData) {
        return std::nullopt;
    }
    assert(headerData->size() == sizeof(PacketHeader));

    auto header = mapDataTo<const PacketHeader>(headerData->data());
    assert(header->packetLengthCaptured <= header->packetLengthOriginal);

    auto packetData = m_reader->nextChunk(header->packetLengthCaptured);
    assert(packetData.has_value());
    assert(packetData->size() == header->packetLengthCaptured);

    const auto timestamp = Packet::Timestamp
    {
        std::chrono::seconds{header->timestampSeconds}
        +
        std::chrono::nanoseconds{header->timestampMicroseconds * 1'000}
    };

    return Packet{std::move(*packetData), timestamp};
}

}

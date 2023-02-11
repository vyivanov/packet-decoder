#include "packet-decoder/simba.hpp"

#include <bit>
#include <cassert>
#include <memory>
#include <optional>
#include <utility>

#include <nlohmann/json.hpp>

#include "packet-decoder/base.hpp"
#include "utils.hpp"

namespace {

#define WA_PARSE_ONE_MESSAGE
#define WA_STUB_MESSAGE_DATA

static_assert(std::endian::native == std::endian::little, "cover big endian architecture");

template <typename T>
auto parseMarketDataHeader(T fromData, nlohmann::ordered_json& toJson) -> T;

template <typename T>
auto parseIncrementalHeader(T fromData, nlohmann::ordered_json& toJson) -> T;

template <typename T>
auto parseSbeMessage(T fromData, nlohmann::ordered_json& toJson) -> T;

}

namespace Pkt {

SimbaPacketDecoder::SimbaPacketDecoder(std::unique_ptr<PacketDecoder> delegator)
    : PacketDecoder(std::move(delegator))
{
    assert(not m_packet);
}

auto SimbaPacketDecoder::decode() -> nlohmann::ordered_json
{
    if (not m_packet) {
        m_packet = m_delegator->payload();
    }

    assert(m_packet);

    auto data = m_packet->data();
    auto json = nlohmann::ordered_json{};

    data = parseMarketDataHeader(data, json);
    data = parseIncrementalHeader(data, json);

    while (data < (m_packet->data() + m_packet->size())) {
        data = parseSbeMessage(data, json);
#ifdef WA_PARSE_ONE_MESSAGE
        data = m_packet->data() + m_packet->size();
#endif
    }

    assert(data == (m_packet->data() + m_packet->size()));
    return json;
}

auto SimbaPacketDecoder::payloadImpl() -> std::optional<Payload>
{
    return std::nullopt;
}

}

namespace {

#pragma pack(1)
struct MarketDataHeader {
    std::uint32_t sequenceNumber;
    std::uint16_t totalLength;
    std::uint16_t flags;
    enum Flag {
        lastFragment = 0,
        startOfSnapshot = 1,
        endOfSnapshot = 2,
        incrementalPacket = 3,
    };
    std::uint64_t sendingTime;
};
static_assert(sizeof(MarketDataHeader) == 16U);
struct IncrementalHeader {
    std::uint64_t transactTime;
    std::uint32_t tradingSessionID;
};
static_assert(sizeof(IncrementalHeader) == 12U);
struct SbeHeader {
    std::uint16_t blockLength;
    std::uint16_t templateID;
    std::uint16_t schemaID;
    std::uint16_t version;
};
static_assert(sizeof(SbeHeader) == 8U);
#pragma pack(0)

inline auto testMessage(MarketDataHeader::Flag flag, std::uint16_t value) -> bool
{
    return ((value bitand (1 << flag)) >> flag);
}

template <typename T>
auto parseMarketDataHeader(T fromData, nlohmann::ordered_json& toJson) -> T
{
    auto header = Pkt::mapDataTo<const MarketDataHeader>(fromData);
    assert(header);

    const auto [seconds, nanoseconds] = Pkt::splitUnixEpoch(header->sendingTime);

    toJson["MarketDataHeader"] = {
        {"SequenceNumber", header->sequenceNumber},
        {"TotalLength", header->totalLength},
        {"Flags", {
            {"LastFragment", testMessage(MarketDataHeader::Flag::lastFragment, header->flags)},
            {"StartOfSnapshot", testMessage(MarketDataHeader::Flag::startOfSnapshot, header->flags)},
            {"EndOfSnapshot", testMessage(MarketDataHeader::Flag::endOfSnapshot, header->flags)},
            {"IncrementalPacket", testMessage(MarketDataHeader::Flag::incrementalPacket, header->flags)}}},
        {"SendingTime", {
            {"Seconds", seconds},
            {"Nanoseconds", nanoseconds}}
        }
    };

    return fromData + sizeof(MarketDataHeader);
}

template <typename T>
auto parseIncrementalHeader(T fromData, nlohmann::ordered_json& toJson) -> T
{
    if (not toJson["MarketDataHeader"]["Flags"]["IncrementalPacket"]) {
        return fromData;
    }

    auto header = Pkt::mapDataTo<const IncrementalHeader>(fromData);
    assert(header);

    const auto [seconds, nanoseconds] = Pkt::splitUnixEpoch(header->transactTime);

    toJson["IncrementalHeader"] = {
        {"TransactTime", {
            {"Seconds", seconds},
            {"Nanoseconds", nanoseconds}}},
        {"TradingSessionID", header->tradingSessionID}
    };

    if (toJson["IncrementalHeader"]["TradingSessionID"] == std::numeric_limits<std::uint32_t>::max()) {
        toJson["IncrementalHeader"]["TransactTime"] = "null";
        toJson["IncrementalHeader"]["TradingSessionID"] = "null";
    }

    return fromData + sizeof(IncrementalHeader);
}

template <typename T>
auto parseSbeMessage(T fromData, nlohmann::ordered_json& toJson) -> T
{
    auto header = Pkt::mapDataTo<const SbeHeader>(fromData);
    assert(header);

    toJson["SbeMessages"].push_back(nlohmann::ordered_json {
        {"SbeHeader", {
            {"BlockLength", header->blockLength},
            {"TemplateID", header->templateID},
            {"SchemaID", header->schemaID},
            {"Version", header->version}}},
#ifdef WA_STUB_MESSAGE_DATA
        {"SbeData", "TBD"}
#endif
    });

#ifdef WA_PARSE_ONE_MESSAGE
    return nullptr;
#endif
}

}

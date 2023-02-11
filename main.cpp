#include <cassert>
#include <cstdio>
#include <memory>
#include <unordered_set>
#include <utility>

#include "data-reader/iface.hpp"
#include "data-reader/local-file.hpp"
#include "format-dissector/iface.hpp"
#include "format-dissector/pcap.hpp"
#include "packet-decoder/ethernet.hpp"
#include "packet-decoder/ip.hpp"
#include "packet-decoder/simba.hpp"
#include "packet-decoder/udp.hpp"
#include "utils.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::fprintf(stderr,
            "Usage:\n  %s <path/to/pcap/file>\n", argv[0]);
        return -1;
    }

    const auto path = argv[1];
    const auto messageIdFilter = std::unordered_set{5, 6, 7};

    Pkt::DataReader::Ptr file = std::make_unique<Pkt::LocalFileReader>(path);
    Pkt::FormatDissector::Ptr pcap = std::make_unique<Pkt::PcapFormatDissector>(std::move(file));

    assert(pcap);
    auto packet = decltype(pcap->nextPacket()){};

    while ((packet = pcap->nextPacket())) {
        assert(packet);
        auto simba = std::make_unique<Pkt::SimbaPacketDecoder>(
            std::make_unique<Pkt::UdpPacketDecoder>(
                std::make_unique<Pkt::IpPacketDecoder>(
                    std::make_unique<Pkt::EthernetPacketDecoder>(
                        std::move(packet->data)
                    )
                )
            )
        );
        assert(simba);
        const auto json = simba->decode();
        for (const auto& message: json["SbeMessages"]) {
            if (messageIdFilter.contains(message["SbeHeader"]["TemplateID"])) {
                const auto [seconds, nanoseconds] = Pkt::splitUnixEpoch(packet->timestamp);
                std::fprintf(stdout, "SBE message from PCAP packet-%010lu.%09lu: %s\n",
                    seconds, nanoseconds, message.dump().c_str()
                );
            }
        }
    }
}

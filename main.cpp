#include <cassert>
#include <cstdio>
#include <memory>
#include <utility>

#include "data-reader/iface.hpp"
#include "data-reader/local-file.hpp"
#include "format-dissector/iface.hpp"
#include "format-dissector/pcap.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::fprintf(stderr,
            "Usage:\n  %s <path/to/pcap/file>\n", argv[0]);
        return -1;
    }

    const auto path = argv[1];

    Pkt::DataReader::Ptr file = std::make_unique<Pkt::LocalFileReader>(path);
    Pkt::FormatDissector::Ptr pcap = std::make_unique<Pkt::PcapFormatDissector>(std::move(file));

    assert(pcap);
    auto packet = decltype(pcap->nextPacket()){};

    while ((packet = pcap->nextPacket())) {
        assert(packet);
    }
}

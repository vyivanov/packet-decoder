#include <cassert>
#include <cstdio>
#include <memory>

#include "data-reader/iface.hpp"
#include "data-reader/local-file.hpp"


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::fprintf(stderr,
            "Usage:\n  %s <path/to/pcap/file>\n", argv[0]);
        return -1;
    }

    const auto path = argv[1];

    Pkt::DataReader::Ptr file = std::make_unique<Pkt::LocalFileReader>(path);

    auto chunk = decltype(file->nextChunk({})){};
    while ((chunk = file->nextChunk(512))) {
        assert(chunk);
    }
}

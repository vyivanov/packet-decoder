#include <memory>

#include "data-reader/iface.hpp"
#include "data-reader/local-file.hpp"


int main()
{
    Pkt::DataReader::Ptr tmp = std::make_unique<Pkt::LocalFileReader>("");
    tmp->nextChunk({});
}

cmake_minimum_required(VERSION 3.18)
project(packet-decoder LANGUAGES CXX)

set(INC inc/data-reader/iface.hpp
        inc/data-reader/local-file.hpp
        inc/format-dissector/iface.hpp
        inc/format-dissector/pcap.hpp
        inc/packet-decoder/base.hpp
        inc/utils.hpp)

set(SRC src/data-reader/local-file.cpp
        src/format-dissector/pcap.cpp
        src/packet-decoder/base.cpp)

add_executable(${PROJECT_NAME} ${INC} ${SRC} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -std=c++20 -Wall -Wextra -Wpedantic -Werror)
target_include_directories(${PROJECT_NAME} PRIVATE inc/)

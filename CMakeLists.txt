cmake_minimum_required(VERSION 3.18)
project(packet-decoder LANGUAGES CXX)

set(INC inc/data-reader/iface.hpp
        inc/data-reader/local-file.hpp
        inc/format-dissector/iface.hpp)

set(SRC src/data-reader/local-file.cpp)

add_executable(${PROJECT_NAME} ${INC} ${SRC} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -std=c++20 -Wall -Wextra -Wpedantic -Werror)
target_include_directories(${PROJECT_NAME} PRIVATE inc/)
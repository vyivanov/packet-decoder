cmake_minimum_required(VERSION 3.18)
project(packet-decoder LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -std=c++20 -Wall -Wextra -Wpedantic -Werror)

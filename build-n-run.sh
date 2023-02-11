#!/usr/bin/env bash

set -u

readonly BUILD_DIR='build-release'
readonly PCAP_FILE="${1}"
readonly TEXT_FILE="$(basename "${PCAP_FILE}")".txt

function build()
{
  rm --force --recursive ${BUILD_DIR}
  cmake -G Ninja -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Release && cmake --build ${BUILD_DIR}
}

function run()
{
  rm --force ${TEXT_FILE}
  ${BUILD_DIR}/packet-decoder "${PCAP_FILE}" > "${TEXT_FILE}"
}

build && run

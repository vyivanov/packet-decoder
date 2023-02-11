#pragma once

#include <chrono>
#include <type_traits>

namespace Pkt {

template <typename T, typename U>
inline auto mapDataTo(U ptr) -> std::add_pointer_t<T>
{
    static_assert(std::is_trivial_v<T>);
    static_assert(std::is_pointer_v<U>);

    return reinterpret_cast<decltype(mapDataTo<T, U>(nullptr))>(ptr);
}

template <typename T>
auto splitUnixEpoch(T timestamp)
{
    constexpr auto kDecimalPoint = 1'000'000'000;

    if constexpr (std::is_same_v<T, std::chrono::utc_time<std::chrono::nanoseconds>>) {
        auto seconds = (timestamp.time_since_epoch().count() / kDecimalPoint);
        auto nanoseconds = (timestamp.time_since_epoch().count() % kDecimalPoint);

        return std::pair{seconds, nanoseconds};
    } else {
        auto seconds = (timestamp / kDecimalPoint);
        auto nanoseconds = (timestamp % kDecimalPoint);

        return std::pair{seconds, nanoseconds};
    }
}

}

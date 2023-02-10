#pragma once

#include <type_traits>

namespace Pkt {

template <typename T, typename U>
inline auto mapDataTo(U ptr) -> std::add_pointer_t<T>
{
    static_assert(std::is_trivial_v<T>);
    static_assert(std::is_pointer_v<U>);

    return reinterpret_cast<decltype(mapDataTo<T, U>(nullptr))>(ptr);
}

}

#pragma once
#ifdef __cplusplus


#include <cstdint>

// FNV-1a 32bit hashing algorithm.
constexpr std::uint32_t hash_fnv1a_32(char const* s, std::size_t count)
{
    return ((count ? hash_fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
{
    return hash_fnv1a_32(s, count);
}

#endif

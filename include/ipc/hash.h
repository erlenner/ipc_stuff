#pragma once
#ifdef __cplusplus

#include "stdint.h"

// FNV-1a 32bit hashing algorithm.
constexpr uint32_t hash_fnv1a_32(const char* s, size_t count)
{
  return ((count ? hash_fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

constexpr uint32_t operator ""_hash(const char* s, size_t count)
{
  return hash_fnv1a_32(s, count);
}

#endif

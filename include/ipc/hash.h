#pragma once
#ifdef __cplusplus

//// constexpr string
//// https://github.com/arcolight/ecl/blob/master/include/ecl/str_const.hpp
//class str_const
//{
//private:
//  const char* const p_;
//  const std::size_t sz_;
//public:
//  template<std::size_t N>
//  constexpr str_const(const char (&a)[N]) :
//    p_(a), sz_(N - 1)
//  {}
//  constexpr char operator[](std::size_t n)
//  {
//    return n < sz_ ? p_[n] : 0;
//  }
//  constexpr std::size_t size() {
//    return sz_;
//  } // size()
//};


//#define const_str(string_literal) 
//
//template <char c>
//class char_t
//{
//  char character = c;
//}
//
//template <typename... char_ts>
//class string_t
//{
//  
//}
//
//auto create_string_t(const char* s)
//{
//  return 
//}


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

//constexpr std::uint32_t check = "0123456789ABCDEF"_hash;


#endif

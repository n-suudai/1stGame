#pragma once

#include "SHeapAllocator.hpp"
#include <string>
#include <sstream>

namespace NE
{

// HeapBasicString
template <typename Elem, typename Traits = std::char_traits<Elem>,
          typename Alloc = STL::SHeapAllocator<Elem>>
using HeapBasicString = std::basic_string<Elem, Traits, Alloc>;

// HeapString
typedef HeapBasicString<char> HeapString;

// HeapBasicStringStream
template <typename Elem, typename Traits = std::char_traits<Elem>,
          typename Alloc = STL::SHeapAllocator<Elem>>
using HeapBasicStringStream = std::basic_stringstream<Elem, Traits, Alloc>;

// HeapStringStream
typedef HeapBasicStringStream<char> HeapStringStream;

// HeapBasicIStringStream
template <typename Elem, typename Traits = std::char_traits<Elem>,
          typename Alloc = STL::SHeapAllocator<Elem>>
using HeapBasicIStringStream = std::basic_istringstream<Elem, Traits, Alloc>;

// HeapIStringStream
typedef HeapBasicIStringStream<char> HeapIStringStream;

// HeapBasicOStringStream
template <typename Elem, typename Traits = std::char_traits<Elem>,
          typename Alloc = STL::SHeapAllocator<Elem>>
using HeapBasicOStringStream = std::basic_ostringstream<Elem, Traits, Alloc>;

// HeapIStringStream
typedef HeapBasicOStringStream<char> HeapOStringStream;

} // namespace NE


#pragma once

#include "../Types.hpp"

#define MEMORY_ALLOCATOR_NEDPOOLING 1
#define MEMORY_ALLOCATOR_NED 2
#define MEMORY_ALLOCATOR_STD 3

#define MEMORY_ALLOCATOR MEMORY_ALLOCATOR_NEDPOOLING
//#define MEMORY_ALLOCATOR MEMORY_ALLOCATOR_NED
//#define MEMORY_ALLOCATOR MEMORY_ALLOCATOR_STD

#define SIMD_ALIGNMENT 128
#define USE_HEAP_TRACKING 1 // Heapトラッキング機能を使用

#ifndef DECL_MALLOC

#if defined(_MSC_VER)

#define DECL_MALLOC __declspec(restrict) __declspec(noalias)

#else // !defined(_MSC_VER)

#define DECL_MALLOC __attribute__((malloc))

#endif

#endif // DECL_MALLOC


#pragma once

#include "MemoryConfig.hpp"

#if MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_NEDPOOLING

#include "Policy/NedPoolingAllocatePolicy.hpp"

namespace NE
{
typedef NedPoolingAllocatePolicy AllocatePolicy;

template <SizeT Alignment = 0>
using AlignAllocatePolicy = NedPoolingAlignedAllocatePolicy<Alignment>;

} // namespace NE

#elif MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_NED

#include "Policy/NedAllocatePolicy.hpp"

namespace NE
{
typedef NedAllocatePolicy AllocatePolicy;

template <SizeT Alignment = 0>
using AlignAllocatePolicy = NedAlignedAllocatePolicy<Alignment>;

} // namespace NE

#elif MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_STD

#include "Policy/StandardAllocatePolicy.hpp"

namespace NE
{
typedef StandardAllocatePolicy AllocatePolicy;

template <SizeT Alignment = 0>
using AlignAllocatePolicy = StandardAlignedAllocatePolicy<Alignment>;

} // namespace NE

#else

#error "does not defined allocator."

#endif

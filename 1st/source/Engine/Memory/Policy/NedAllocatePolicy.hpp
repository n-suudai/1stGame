
/*

This source code was created with reference to Ogre.

(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#pragma once

#include "../MemoryConfig.hpp"

#if MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_NED

namespace NE
{

class NedAllocatePolicyImpl
{
public:
    static void* AllocateBytes(SizeT bytes);

    static void DeallocateBytes(void* pBlock);

    static void* AllocateBytesAligned(SizeT alignment, SizeT bytes);

    static void DeallocateBytesAligned(SizeT alignment, void* pBlock);
};

class NedAllocatePolicy
{
public:
    static inline void* AllocateBytes(SizeT bytes)
    {
        return NedAllocatePolicyImpl::AllocateBytes(bytes);
    }

    static inline void DeallocateBytes(void* pBlock)
    {
        NedAllocatePolicyImpl::DeallocateBytes(pBlock);
    }
};

template <SizeT Alignment = 0>
class NedAlignedAllocatePolicy
{
    // compile-time check alignment is available.
    typedef S32 IsValidAlignment
      [Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];

public:
    static inline void* AllocateBytes(SizeT bytes)
    {
        return NedAllocatePolicyImpl::AllocateBytesAligned(Alignment, bytes);
    }

    static inline void DeallocateBytes(void* pBlock)
    {
        NedAllocatePolicyImpl::DeallocateBytesAligned(Alignment, pBlock);
    }
};

} // namespace NE

#endif // MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_NED

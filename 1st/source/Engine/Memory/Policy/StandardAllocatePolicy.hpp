
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

#if MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_STD

#include "../AlignedAllocator.hpp"

namespace NE
{

class StandardAllocatePolicy
{
public:
    static inline void* AllocateBytes(SizeT bytes)
    {
        return reinterpret_cast<void*>(new unsigned char[bytes]);
    }

    static inline void DeallocateBytes(void* pBlock)
    {
        // deal with null
        if (!pBlock)
        {
            return;
        }

        delete[](reinterpret_cast<unsigned char*>(pBlock));
    }
};

template <SizeT Alignment>
class StandardAlignedAllocatePolicy
{
    // compile-time check alignment is available.
    typedef S32 IsValidAlignment
      [Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];

public:
    static inline void* AllocateBytes(SizeT bytes)
    {
        return AlignedMemory::Allocate(bytes, Alignment);
    }

    static inline void DeallocateBytes(void* pBlock)
    {
        // deal with null
        if (!pBlock)
        {
            return;
        }

        AlignedMemory::Deallocate(pBlock);
    }
};

} // namespace NE

#endif // MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_STD

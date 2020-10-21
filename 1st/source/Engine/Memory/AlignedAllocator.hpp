
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

#include "MemoryConfig.hpp"
#include <assert.h>
#include <cstddef>

namespace NE
{

template <typename T>
bool IsPowerOf2(T value)
{
    return !(value & (value - 1));
}

/**
 *
 * |___2___|3|_________5__________|__6__|
 * ^         ^
 * 1         4
 *
 * 1 -> Pointer to start of the block allocated by new.
 * 2 -> Gap used to get 4 aligned on given alignment
 * 3 -> Byte offset between 1 and 4
 * 4 -> Pointer to the start of data block.
 * 5 -> Data block.
 * 6 -> Wasted memory at rear of data block.
 */

class AlignedMemory
{
public:
    static inline void* Allocate(SizeT bytes, SizeT alignment)
    {
        assert(0 < alignment && alignment <= 128 && IsPowerOf2(alignment));

        U8* p = new U8[bytes + alignment];
        std::ptrdiff_t offset =
          alignment - (reinterpret_cast<std::ptrdiff_t>(p) & (alignment - 1));

        U8* pBlock = p + offset;
        pBlock[-1] = static_cast<U8>(offset);

        return pBlock;
    }

    static inline void* Allocate(SizeT bytes)
    {
        return Allocate(bytes, SIMD_ALIGNMENT);
    }

    static inline void Deallocate(void* pBlock)
    {
        if (pBlock)
        {
            U8* p = reinterpret_cast<U8*>(pBlock);
            p = p - p[-1];
            delete[] p;
        }
    }
};

} // namespace NE

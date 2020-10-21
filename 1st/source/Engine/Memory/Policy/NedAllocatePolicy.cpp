
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


#include "NedAllocatePolicy.hpp"

#if MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_NED

#include "NedMalloc/NedMallocInc.hpp"

namespace NE
{

void* NedAllocatePolicyImpl::AllocateBytes(SizeT bytes)
{
    return nedalloc::nedmalloc(bytes);
}

void NedAllocatePolicyImpl::DeallocateBytes(void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    nedalloc::nedfree(pBlock);
}

void* NedAllocatePolicyImpl::AllocateBytesAligned(SizeT align, SizeT bytes)
{
    // default to platform SIMD alignment if none specified
    return nedalloc::nedmemalign(align ? align : SIMD_ALIGNMENT, bytes);
}

void NedAllocatePolicyImpl::DeallocateBytesAligned(SizeT alignment,
                                                   void* pBlock)
{
    alignment;

    // deal with null
    if (!pBlock)
    {
        return;
    }

    nedalloc::nedfree(pBlock);
}

} // namespace NE

#endif // MEMORY_ALLOCATOR == MEMORY_ALLOCATOR_NED

//
// Created by bullets_2019_0305 on 2019/03/15.
//

#pragma once

#include "../Heap.hpp"
#include "../HeapFactory.hpp"
#include "HeapAllocator_NonTracking.hpp"

namespace NE
{
#if USE_HEAP_TRACKING

#ifndef STL_HEAP_NAME
#define STL_HEAP_NAME "STL"
#endif

namespace STL
{
//=============================================================================
// 追跡有りのアロケータ
//=============================================================================
template <SizeT Alignment>
struct SHeapAllocatorBase
{
    static Heap* s_pHeap;

    static inline void* Allocate(size_t bytes)
    {
        if (s_pHeap == nullptr)
        {
            s_pHeap = HeapFactory::Get().CreateHeap(STL_HEAP_NAME);
        }
        return s_pHeap->Allocate<AlignAllocatePolicy<Alignment>>(
          bytes, __FILE__, __LINE__, __FUNCTION__);
    }

    static inline void Deallocate(void* pBlock)
    {
        s_pHeap->Deallocate<AlignAllocatePolicy<Alignment>>(pBlock);
    }
};

template <SizeT Alignment>
Heap* SHeapAllocatorBase<Alignment>::s_pHeap = nullptr;

//=============================================================================
// 追跡有りのアロケータ（SharedPtr用）
//=============================================================================
template <typename T>
struct SHeapSharedAllocator
{
    Heap* m_pHeap;
    using value_type = T;

    template <typename U>
    struct rebind
    {
        typedef SHeapSharedAllocator<U> other;
    };

    inline SHeapSharedAllocator(Heap* pHeap) : m_pHeap(pHeap)
    { /* DO_NOTHING */
    }

    template <typename U>
    inline SHeapSharedAllocator(const SHeapSharedAllocator<U>& other)
    { /* DO_NOTHING */
        m_pHeap = other.m_pHeap;
    }

    inline T* allocate(size_t count)
    {
        return reinterpret_cast<T*>(
          m_pHeap->Allocate<AlignAllocatePolicy<alignof(T)>>(
            sizeof(T) * count, __FILE__, __LINE__, __FUNCTION__));
    }

    inline void deallocate(T* ptr, size_t)
    {
        m_pHeap->Deallocate<AlignAllocatePolicy<alignof(T)>>(ptr);
    }
};

template <typename T, typename U>
inline bool operator==(const SHeapSharedAllocator<T>&,
                       const SHeapSharedAllocator<U>&)
{
    return true;
}

template <typename T, typename U>
inline bool operator!=(const SHeapSharedAllocator<T>&,
                       const SHeapSharedAllocator<U>&)
{
    return false;
}

} // namespace STL

#else // !USE_HEAP_TRACKING

namespace STL
{
template <SizeT Alignment>
using SHeapAllocatorBase = SHeapAllocatorBase_NonTracking<Alignment>;

template <typename T>
using SHeapSharedAllocator = SHeapSharedAllocator_NonTracking<T>;

} // namespace STL

#endif // USE_HEAP_TRACKING

namespace STL
{
//=============================================================================
// STLアロケータ
//=============================================================================
template <typename T>
struct SHeapAllocator : public SHeapAllocatorBase<alignof(T)>
{
    using super = SHeapAllocatorBase<alignof(T)>;
    using value_type = T;

    template <typename U>
    struct rebind
    {
        typedef SHeapAllocator<U> other;
    };

    inline SHeapAllocator()
    { /* DO_NOTHING */
    }

    template <typename U>
    inline SHeapAllocator(const SHeapAllocator<U>&)
    { /* DO_NOTHING */
    }

    inline T* allocate(size_t count)
    {
        return reinterpret_cast<T*>(super::Allocate(sizeof(T) * count));
    }

    inline void deallocate(T* ptr, size_t)
    {
        super::Deallocate(reinterpret_cast<void*>(ptr));
    }
};

template <typename T, typename U>
inline bool operator==(const SHeapAllocator<T>&, const SHeapAllocator<U>&)
{
    return true;
}

template <typename T, typename U>
inline bool operator!=(const SHeapAllocator<T>&, const SHeapAllocator<U>&)
{
    return false;
}

} // namespace STL
} // namespace NE

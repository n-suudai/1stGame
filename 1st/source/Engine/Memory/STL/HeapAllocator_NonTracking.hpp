#pragma once

#include "../AllocateConfig.hpp"
#include <cstddef>
#include <memory>

namespace NE
{
namespace STL
{
//=============================================================================
// 追跡無しのアロケータ
//=============================================================================
template <SizeT Alignment>
struct SHeapAllocatorBase_NonTracking
{
    static inline void* Allocate(size_t bytes)
    {
        return AlignAllocatePolicy<Alignment>::AllocateBytes(bytes);
    }

    static inline void Deallocate(void* pBlock)
    {
        AlignAllocatePolicy<Alignment>::DeallocateBytes(pBlock);
    }
};

//=============================================================================
// 追跡無しのアロケータ（SharedPtr用）
//=============================================================================
template <typename T>
struct SHeapSharedAllocator_NonTracking
{
    using value_type = T;

    template <typename U>
    struct rebind
    {
        typedef SHeapSharedAllocator_NonTracking<U> other;
    };

    inline SHeapSharedAllocator_NonTracking()
    {/* DO_NOTHING */
    }

    template <typename U>
    inline SHeapSharedAllocator_NonTracking(
      const SHeapSharedAllocator_NonTracking<U>&)
    {/* DO_NOTHING */
    }

    inline T* allocate(size_t count)
    {
        return reinterpret_cast<T*>(
          AlignAllocatePolicy<alignof(T)>::AllocateBytes(sizeof(T) * count));
    }

    inline void deallocate(T* ptr, size_t)
    {
        AlignAllocatePolicy<alignof(T)>::DeallocateBytes(ptr);
    }
};

template <typename T, typename U>
inline bool operator==(const SHeapSharedAllocator_NonTracking<T>&,
                       const SHeapSharedAllocator_NonTracking<U>&)
{
    return true;
}

template <typename T, typename U>
inline bool operator!=(const SHeapSharedAllocator_NonTracking<T>&,
                       const SHeapSharedAllocator_NonTracking<U>&)
{
    return false;
}

//=============================================================================
// 追跡無しのアロケータ
//=============================================================================
template <typename T>
struct SHeapAllocator_NonTracking
  : public SHeapAllocatorBase_NonTracking<alignof(T)>
{
    using super = SHeapAllocatorBase_NonTracking<alignof(T)>;
    using value_type = T;

    template <typename U>
    struct rebind
    {
        typedef SHeapAllocator_NonTracking<U> other;
    };

    inline SHeapAllocator_NonTracking()
    {/* DO_NOTHING */
    }

    template <typename U>
    inline SHeapAllocator_NonTracking(const SHeapAllocator_NonTracking<U>&)
    {/* DO_NOTHING */
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
inline bool operator==(const SHeapAllocator_NonTracking<T>&,
                       const SHeapAllocator_NonTracking<U>&)
{
    return true;
}

template <typename T, typename U>
inline bool operator!=(const SHeapAllocator_NonTracking<T>&,
                       const SHeapAllocator_NonTracking<U>&)
{
    return false;
}

} // namespace STL
} // namespace NE

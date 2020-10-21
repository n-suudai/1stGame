
#pragma once

#include "AllocateConfig.hpp"
#include "Heap.hpp"
#include "HeapFactory.hpp"
#include "STL/SHeapAllocator.hpp"
#include <cassert>

#if USE_HEAP_TRACKING

namespace NE
{

inline void* Malloc(SizeT bytes, const char* file, const S32 line,
                    const char* function)
{
    static Heap* pDefault = HeapFactory::Get().GetDefaultHeap();
    return pDefault->Allocate<AllocatePolicy>(bytes, file, line, function);
}

inline void* MallocHeap(Heap* pHeap, SizeT bytes, const char* file,
                        const S32 line, const char* function)
{
    assert(pHeap != nullptr);
    return pHeap->Allocate<AllocatePolicy>(bytes, file, line, function);
}

inline void Free(void* pBlock)
{
    if (!pBlock)
    {
        return;
    }
    static Heap* pDefault = HeapFactory::Get().GetDefaultHeap();
    pDefault->Deallocate<AllocatePolicy>(pBlock);
}

inline void FreeHeap(Heap* pHeap, void* pBlock)
{
    if (!pBlock)
    {
        return;
    }
    assert(pHeap != nullptr);
    pHeap->Deallocate<AllocatePolicy>(pBlock);
}

template <SizeT Alignment>
inline void* MallocAligned(SizeT bytes, const char* file, const S32 line,
                           const char* function)
{
    static Heap* pDefault = HeapFactory::Get().GetDefaultHeap();
    return pDefault->Allocate<AlignAllocatePolicy<Alignment>>(bytes, file, line,
                                                              function);
}

template <SizeT Alignment>
inline void* MallocAlignedHeap(Heap* pHeap, SizeT bytes, const char* file,
                               const S32 line, const char* function)
{
    assert(pHeap != nullptr);
    return pHeap->Allocate<AlignAllocatePolicy<Alignment>>(bytes, file, line,
                                                           function);
}

template <SizeT Alignment>
inline void FreeAligned(void* pBlock)
{
    if (!pBlock)
    {
        return;
    }
    static Heap* pDefault = HeapFactory::Get().GetDefaultHeap();
    pDefault->Deallocate<AlignAllocatePolicy<Alignment>>(pBlock);
}

template <SizeT Alignment>
inline void FreeAlignedHeap(Heap* pHeap, void* pBlock)
{
    if (!pBlock)
    {
        return;
    }
    assert(pHeap != nullptr);
    pHeap->Deallocate<AlignAllocatePolicy<Alignment>>(pBlock);
}

} // namespace NE

#define DECLARE_HEAP(className)                                            \
                                                                           \
private:                                                                   \
    friend NE::STL::SHeapSharedAllocator<className>;                       \
    static NE::Heap* s_pHeap;                                              \
                                                                           \
public:                                                                    \
    static NE::Heap* GetHeap();                                            \
    static void* operator new(NE::SizeT bytes, const char* file,           \
                              const NE::S32 line, const char* function);   \
    static void* operator new(NE::SizeT bytes);                            \
    static void operator delete(void* pBlock, const char* file,            \
                                const NE::S32 line, const char* function); \
    static void operator delete(void* pBlock);

#define DEFINE_HEAP(className, heapName, policyName)                          \
    NE::Heap* className::s_pHeap = nullptr;                                   \
    void* className::operator new(NE::SizeT bytes, const char* file,          \
                                  const NE::S32 line, const char* function)   \
    {                                                                         \
        return className::GetHeap()->Allocate<policyName>(bytes, file, line,  \
                                                          function);          \
    }                                                                         \
    void* className::operator new(NE::SizeT bytes)                            \
    {                                                                         \
        return className::GetHeap()->Allocate<policyName>(                    \
          bytes, __FILE__, __LINE__, __FUNCTION__);                           \
    }                                                                         \
    void className::operator delete(void* pBlock, const char*, const NE::S32, \
                                    const char*)                              \
    {                                                                         \
        className::GetHeap()->Deallocate<policyName>(pBlock);                 \
    }                                                                         \
    void className::operator delete(void* pBlock)                             \
    {                                                                         \
        className::GetHeap()->Deallocate<policyName>(pBlock);                 \
    }                                                                         \
    NE::Heap* className::GetHeap()                                            \
    {                                                                         \
        if (s_pHeap == nullptr)                                               \
        {                                                                     \
            s_pHeap = NE::HeapFactory::Get().CreateHeap(heapName);            \
        }                                                                     \
        return s_pHeap;                                                       \
    }

#define DEFINE_HIERARCHAL_HEAP(className, heapName, parentName, policyName)    \
    NE::Heap* className::s_pHeap = nullptr;                                    \
    void* className::operator new(NE::SizeT bytes, const char* file,           \
                                  const NE::S32 line, const char* function)    \
    {                                                                          \
        return className::GetHeap()->Allocate<policyName>(bytes, file, line,   \
                                                          function);           \
    }                                                                          \
    void* className::operator new(NE::SizeT bytes)                             \
    {                                                                          \
        return className::GetHeap()->Allocate<policyName>(                     \
          bytes, __FILE__, __LINE__, __FUNCTION__);                            \
    }                                                                          \
    void className::operator delete(void* pBlock, const char*, const NE::S32,  \
                                    const char*)                               \
    {                                                                          \
        className::GetHeap()->Deallocate<policyName>(pBlock);                  \
    }                                                                          \
    void className::operator delete(void* pBlock)                              \
    {                                                                          \
        className::GetHeap()->Deallocate<policyName>(pBlock);                  \
    }                                                                          \
    NE::Heap* className::GetHeap()                                             \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap = NE::HeapFactory::Get().CreateHeap(heapName, parentName); \
        }                                                                      \
        return s_pHeap;                                                        \
    }

#define NEW new (__FILE__, __LINE__, __FUNCTION__)
#define DELETE delete

#define MEM_ALLOC(bytes) NE::Malloc((bytes), __FILE__, __LINE__, __FUNCTION__)
#define MEM_FREE(pBlock) NE::Free((pBlock))

#define MEM_ALLOC_ALIGNED(bytes, alignment) \
    NE::MallocAligned<alignment>((bytes), __FILE__, __LINE__, __FUNCTION__)
#define MEM_FREE_ALIGNED(pBlock, alignment) NE::FreeAligned<alignment>((pBlock))

// クラスの Heap から確保＆解放
#define MEM_ALLOC_HEAP(bytes) \
    NE::MallocHeap(GetHeap(), (bytes), __FILE__, __LINE__, __FUNCTION__)
#define MEM_FREE_HEAP(pBlock) NE::FreeHeap(GetHeap(), (pBlock))

#define MEM_ALLOC_ALIGNED_HEAP(bytes, alignment)                             \
    NE::MallocAlignedHeap<alignment>(GetHeap(), (bytes), __FILE__, __LINE__, \
                                     __FUNCTION__)
#define MEM_FREE_ALIGNED_HEAP(pBlock, alignment) \
    NE::FreeAlignedHeap<alignment>(GetHeap(), (pBlock))

#else // !USE_HEAP_TRACKING

namespace NE
{

inline void* Malloc(SizeT bytes, const char*, const S32, const char*)
{
    return AllocatePolicy::AllocateBytes(bytes);
}

inline void* Malloc(SizeT bytes)
{
    return AllocatePolicy::AllocateBytes(bytes);
}

inline void Free(void* pBlock)
{
    if (!pBlock)
    {
        return;
    }
    AllocatePolicy::DeallocateBytes(pBlock);
}

template <SizeT Alignment>
inline void* MallocAligned(SizeT bytes, const char*, const S32, const char*)
{
    return AlignAllocatePolicy<Alignment>::AllocateBytes(bytes);
}

template <SizeT Alignment>
inline void* MallocAligned(SizeT bytes)
{
    return AlignAllocatePolicy<Alignment>::AllocateBytes(bytes);
}

template <SizeT Alignment>
inline void FreeAligned(void* pBlock)
{
    if (!pBlock)
    {
        return;
    }
    AlignAllocatePolicy<Alignment>::DeallocateBytes(pBlock);
}

} // namespace NE

#define DECLARE_HEAP(className)                                            \
private:                                                                   \
    friend NE::STL::SHeapSharedAllocator<className>;                       \
                                                                           \
public:                                                                    \
    static void* operator new(NE::SizeT bytes, const char* file,           \
                              const NE::S32 line, const char* function);   \
    static void* operator new(NE::SizeT bytes);                            \
    static void operator delete(void* pBlock, const char* file,            \
                                const NE::S32 line, const char* function); \
    static void operator delete(void* pBlock);

#define DEFINE_HEAP(className, heapName, policyName)                           \
    void* className::operator new(NE::SizeT bytes, const char*, const NE::S32, \
                                  const char*)                                 \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    void* className::operator new(NE::SizeT bytes)                             \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    void className::operator delete(void* pBlock, const char*, const NE::S32,  \
                                    const char*)                               \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }                                                                          \
    void className::operator delete(void* pBlock)                              \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }

#define DEFINE_HIERARCHAL_HEAP(className, heapName, parentName, policyName)    \
    void* className::operator new(NE::SizeT bytes, const char*, const NE::S32, \
                                  const char*)                                 \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    void* className::operator new(NE::SizeT bytes)                             \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    void className::operator delete(void* pBlock, const char*, const NE::S32,  \
                                    const char*)                               \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }                                                                          \
    void className::operator delete(void* pBlock)                              \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }

#define NEW new
#define DELETE delete

#define MEM_ALLOC(bytes) NE::Malloc((bytes))
#define MEM_FREE(pBlock) NE::Free((pBlock))

#define MEM_ALLOC_ALIGNED(bytes, alignment) \
    NE::MallocAligned<alignment>((bytes))
#define MEM_FREE_ALIGNED(pBlock, alignment) NE::FreeAligned<alignment>((pBlock))

#define MEM_ALLOC_HEAP(bytes) NE::Malloc((bytes))
#define MEM_FREE_HEAP(pBlock) NE::Free((pBlock))

#define MEM_ALLOC_ALIGNED_HEAP(bytes, alignment) \
    NE::MallocAligned<alignment>((bytes))
#define MEM_FREE_ALIGNED_HEAP(pBlock, alignment) \
    NE::FreeAligned<alignment>((pBlock))

#endif // USE_HEAP_TRACKING

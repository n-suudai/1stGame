
#pragma once

#include "../MemoryConfig.hpp"
#include "../MemoryManager.hpp"
#include "HeapAllocator.hpp"
#include <memory>

namespace NE
{
namespace Detail
{

// カスタムデリータ UniquePtr用
template <typename T>
struct CustomDeleter
{
    inline CustomDeleter() = default;

    template <typename T2,
              std::enable_if_t<std::is_convertible_v<T2 *, T *>, int> = 0>
    inline CustomDeleter(const CustomDeleter<T2> &)
    { // construct from another default_delete
    }

    inline void operator()(T *ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        NE_DELETE ptr;
    }
};

// カスタムデリータ 配列型のUniquePtr用
template <typename T>
struct CustomDeleter<T[]>
{
    inline CustomDeleter() = default;

    template <typename T2, std::enable_if_t<
                             std::is_convertible_v<T2 (*)[], T (*)[]>, int> = 0>
    inline CustomDeleter(const CustomDeleter<T2[]> &)
    { // construct from another default_delete
    }

    template <typename T2, std::enable_if_t<
                             std::is_convertible_v<T2 (*)[], T (*)[]>, int> = 0>
    inline void operator()(T2 *ptr) const
    { // delete a pointer
        static_assert(0 < sizeof(T2), "can't delete an incomplete type");

        NE_DELETE[] ptr;
    }
};

// カスタムデリータ UniqueBlob用
#if USE_HEAP_TRACKING
template <typename T>
struct CustomDeleter_Blob
{
    inline CustomDeleter_Blob(Heap *pHeap) : m_pHeap(pHeap)
    {
    }

    inline CustomDeleter_Blob() = default;

    inline Heap *GetHeap() const
    {
        return m_pHeap;
    }

    template <typename T2,
              std::enable_if_t<std::is_convertible_v<T2 *, T *>, int> = 0>
    inline CustomDeleter_Blob(const CustomDeleter_Blob<T2> &)
    { // construct from another default_delete
    }

    inline void operator()(T *ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        NE_MEM_FREE_HEAP(ptr);
    }

    Heap *m_pHeap;
};
#else  // !USE_HEAP_TRACKING
template <typename T>
struct CustomDeleter_Blob
{
    inline CustomDeleter_Blob() = default;

    template <typename T2,
              std::enable_if_t<std::is_convertible_v<T2 *, T *>, int> = 0>
    inline CustomDeleter_Blob(const CustomDeleter_Blob<T2> &)
    { // construct from another default_delete
    }

    inline void operator()(T *ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        NE_MEM_FREE_HEAP(ptr);
    }
};
#endif // USE_HEAP_TRACKING

} // namespace Detail

// SharedPtr
template <typename T>
using SharedPtr = std::shared_ptr<T>;

// WeakPtr
template <typename T>
using WeakPtr = std::weak_ptr<T>;

// UniquePtr
template <typename T, typename Deleter = Detail::CustomDeleter<T>>
using UniquePtr = std::unique_ptr<T, Deleter>;

// UniqueBlob
using UniqueBlob = std::unique_ptr<NE::U8, Detail::CustomDeleter_Blob<NE::U8>>;

// MakeShared
template <typename T, typename... Arguments>
inline SharedPtr<T> MakeShared(Arguments &&... arguments)
{
    static STL::SHeapSharedAllocator<T> allocator =
      STL::SHeapSharedAllocator<T>(
#if USE_HEAP_TRACKING
        T::GetHeap()
#endif
        );
    return std::allocate_shared<T, STL::SHeapSharedAllocator<T>>(
      allocator, std::forward<Arguments>(arguments)...);
}

// MakeUnique
template <typename T, typename... Arguments,
          std::enable_if_t<!std::is_array_v<T>, int> = 0>
inline UniquePtr<T> MakeUnique(Arguments &&... arguments)
{
    static Detail::CustomDeleter<T> deleter = Detail::CustomDeleter<T>();

    return UniquePtr<T>(NE_NEW T(std::forward<Arguments>(arguments)...),
                        deleter);
}

template <
  typename T,
  std::enable_if_t<std::is_array_v<T> &&std::extent<T>::value == 0, int> = 0>
inline UniquePtr<T> MakeUnique(size_t size)
{
    typedef std::remove_extent_t<T> Element;

    static Detail::CustomDeleter<T> deleter = Detail::CustomDeleter<T>();

    return (UniquePtr<T>(NE_NEW Element[size](), deleter));
}

template <typename T, class... Arguments,
          std::enable_if_t<std::extent<T>::value != 0, int> = 0>
void MakeUnique(Arguments &&...) = delete;

// Blob作成
#if USE_HEAP_TRACKING

UniqueBlob MakeUniqueBlob(SizeT bytes, Heap *pHeap);

#define MAKE_UNIQUE_BLOB(bytes) NE::MakeUniqueBlob((bytes), GetHeap())

#else // !USE_HEAP_TRACKING

UniqueBlob MakeUniqueBlob(SizeT bytes);

#define MAKE_UNIQUE_BLOB(bytes) NE::MakeUniqueBlob((bytes))

#endif // USE_HEAP_TRACKING

} // namespace NE

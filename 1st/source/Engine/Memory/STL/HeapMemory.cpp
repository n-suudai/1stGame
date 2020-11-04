
#include "HeapMemory.hpp"

namespace NE
{
// Blob作成
#if USE_HEAP_TRACKING
UniqueBlob MakeUniqueBlob(SizeT bytes, Heap* pHeap)
{
    Detail::CustomDeleter_Blob<NE::U8> deleter(pHeap);

    auto GetHeap = [pHeap]()->Heap *
    {
        return pHeap;
    };

    return UniqueBlob(reinterpret_cast<NE::U8*>(NE_MEM_ALLOC_HEAP(bytes)),
                      deleter);
}
#else  // !USE_HEAP_TRACKING
UniqueBlob MakeUniqueBlob(SizeT bytes)
{
    Detail::CustomDeleter_Blob<NE::U8> deleter;

    return UniqueBlob(reinterpret_cast<NE::U8*>(NE_MEM_ALLOC_HEAP(bytes)),
                      deleter);
}
#endif // USE_HEAP_TRACKING
}

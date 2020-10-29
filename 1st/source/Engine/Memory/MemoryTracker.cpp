
#include "MemoryTracker.hpp"

#if USE_HEAP_TRACKING


#include "Heap.hpp"
#include "AllocateConfig.hpp"
#include <assert.h>

namespace NE
{

MemoryTracker& MemoryTracker::Get()
{
    static MemoryTracker instance;
    return instance;
}

MemoryTracker::MemoryTracker()
    : m_protection(), m_allocations(), m_nextAllocationBookmark(1)
{
}

MemoryTracker::~MemoryTracker()
{
    // ※この時点で残っているAllocationはリークしている
}

Allocation* MemoryTracker::RecordAllocation(void* pBlock, SizeT bytes,
                                            const char* file, S32 line,
                                            const char* function, Heap* pHeap)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    Allocation* pAllocation = new Allocation();
    pAllocation->pBlock = pBlock;
    pAllocation->bytes = bytes;
    pAllocation->file = file;
    pAllocation->line = line;
    pAllocation->function = function;
    pAllocation->time = 0;
    pAllocation->backTraceHash = 0; /*StackTrace::CaptureStackTraceHash();*/
    pAllocation->pSignature = reinterpret_cast<AllocationSignature*>(
      reinterpret_cast<std::ptrdiff_t>(pBlock) + bytes);
    pAllocation->bookmark = m_nextAllocationBookmark;
    pAllocation->pHeap = pHeap;

    // シグネチャを書き込み
    (*pAllocation->pSignature) = Allocation::SIGNATURE;

    m_allocations[pBlock] = pAllocation;

    // リンクリストを構築
    pHeap->AddAllocation(pAllocation);

    m_nextAllocationBookmark++;

    return pAllocation;
}

void MemoryTracker::RecordDeallocation(void* pBlock, Heap* pHeap)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    auto it = m_allocations.find(pBlock);

    if (it != m_allocations.end())
    {
        // シグネチャをチェック
        assert((*it->second->pSignature) == Allocation::SIGNATURE);

        // リンクリストから切り離す
        pHeap->EraseAllocation(it->second);

        delete it->second;

        m_allocations.erase(it);
    }
}

SizeT MemoryTracker::GetAllocationBookmark() const
{
    return m_nextAllocationBookmark;
}

} // namespace NE

#endif // USE_HEAP_TRACKING



#include "Heap.hpp"

#if USE_HEAP_TRACKING

#include "AllocateConfig.hpp"
#include "HeapWalk.hpp"
#include <assert.h>

namespace NE
{

void* Allocation::operator new(NE::SizeT bytes)
{
    return AllocatePolicy::AllocateBytes(bytes);
}

void Allocation::operator delete(void* pBlock)
{
    AllocatePolicy::DeallocateBytes(pBlock);
}

Heap::Heap()
    : m_protection()
    , m_totalAllocatedBytes(0)
    , m_peakAllocatedBytes(0)
    , m_allocatedInstanceCount(0)
    , m_pAllocation(nullptr)
    , m_pParent(nullptr)
    , m_pFirstChild(nullptr)
    , m_pNextSibling(nullptr)
    , m_pPrevSibling(nullptr)
    , m_isActive(false)
    , m_name()
{
}

Heap::~Heap() {}

void Heap::Initialize()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    m_isActive = false;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
    m_pAllocation = nullptr;
    m_pParent = nullptr;
    m_pFirstChild = nullptr;
    m_pNextSibling = nullptr;
    m_pPrevSibling = nullptr;
}

void Heap::Activate(const char* name)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    assert(name != nullptr);
    assert(strlen(name) < NAMELENGTH);
    strcpy_s(m_name, name);
    m_isActive = true;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
}

void Heap::Deactivate()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    strcpy_s(m_name, "");
    m_isActive = false;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
}

const char* Heap::GetName() const { return m_name; }

bool Heap::IsActive() const { return m_isActive; }

// リンクリストを構築
void Heap::AddAllocation(Allocation* pAllocation)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    pAllocation->pPrev = nullptr;
    pAllocation->pNext = m_pAllocation;

    if (m_pAllocation != nullptr)
    {
        m_pAllocation->pPrev = pAllocation;
    }

    m_pAllocation = pAllocation;

    // 確保サイズ情報更新
    m_totalAllocatedBytes += pAllocation->bytes;

    if (m_totalAllocatedBytes > m_peakAllocatedBytes)
    {
        m_peakAllocatedBytes = m_totalAllocatedBytes;
    }

    m_allocatedInstanceCount++;
}

// リンクリストから切り離す
void Heap::EraseAllocation(Allocation* pAllocation)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (pAllocation->pPrev == nullptr)
    {
        assert(pAllocation == m_pAllocation);
        m_pAllocation = pAllocation->pNext;
    }
    else
    {
        pAllocation->pPrev->pNext = pAllocation->pNext;
    }

    if (pAllocation->pNext != nullptr)
    {
        pAllocation->pNext->pPrev = pAllocation->pPrev;
    }

    // 確保サイズ情報更新
    m_totalAllocatedBytes -= pAllocation->bytes;
    m_allocatedInstanceCount--;
}

// 親子関係の構築をする関数
void Heap::AttachTo(Heap* pParent)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    assert(pParent != nullptr);

    if (pParent == m_pParent)
    {
        return;
    }

    // 1. 現在の親子関係を切り離す
    if (m_pPrevSibling != nullptr)
    {
        m_pPrevSibling->m_pNextSibling = m_pNextSibling;
    }

    if (m_pNextSibling != nullptr)
    {
        m_pNextSibling->m_pPrevSibling = m_pPrevSibling;
    }

    if (m_pParent != nullptr)
    {
        if (m_pParent->m_pFirstChild == this)
        {
            m_pParent->m_pFirstChild = m_pNextSibling;
        }
    }

    // 2. 新しく親子関係を構築
    m_pNextSibling = pParent->m_pFirstChild;
    m_pPrevSibling = nullptr;
    m_pParent = pParent;
    pParent->m_pFirstChild = this;
}

// リークのチェック関数
void Heap::MemoryLeakCheck(IMemoryLeakReporter* pReporter, SizeT bookmarkStart,
                           SizeT bookmarkEnd) const
{
    assert(pReporter != nullptr);

    Allocation* pAllocation = m_pAllocation;
    while (pAllocation != nullptr)
    {
        if (pAllocation->bookmark >= bookmarkStart &&
            pAllocation->bookmark <= bookmarkEnd)
        {
            pReporter->Report(this, pAllocation);
        }
        pAllocation = pAllocation->pNext;
    }
}

// 情報収集のための関数
void Heap::ReportTreeStats(IHeapTreeStatsReporter* pAccumulator,
                           S32 depth) const
{
    assert(pAccumulator != nullptr);

    IHeapTreeStatsReporter::HeapTreeStats local, total;

    local.totalBytes = m_totalAllocatedBytes;
    local.peakBytes = m_peakAllocatedBytes;
    local.instanceCount = m_allocatedInstanceCount;

    GetTreeStats(total.totalBytes, total.peakBytes, total.instanceCount);

    pAccumulator->Report(depth, this, local, total);

    Heap* pChild = m_pFirstChild;
    while (pChild != nullptr)
    {
        pChild->ReportTreeStats(pAccumulator, depth + 1);
        pChild = pChild->m_pNextSibling;
    }
}

// メモリ破壊のチェック関数
void Heap::MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                                SizeT bookmarkStart, SizeT bookmarkEnd) const
{
    assert(pReporter != nullptr);

    Allocation* pAllocation = m_pAllocation;
    while (pAllocation != nullptr)
    {
        if (pAllocation->bookmark >= bookmarkStart &&
            pAllocation->bookmark <= bookmarkEnd)
        {
            if ((*pAllocation->pSignature) != Allocation::SIGNATURE)
            {
                pReporter->Report(this, pAllocation);
            }
        }
        pAllocation = pAllocation->pNext;
    }
}

void Heap::GetTreeStats(SizeT& totalBytes, SizeT& totalPeakBytes,
                        SizeT& totalInstanceCount) const
{
    totalBytes += m_totalAllocatedBytes;
    totalPeakBytes += m_peakAllocatedBytes;
    totalInstanceCount += m_allocatedInstanceCount;

    Heap* pChild = m_pFirstChild;
    while (pChild != NULL)
    {
        pChild->GetTreeStats(totalBytes, totalPeakBytes, totalInstanceCount);
        pChild = pChild->m_pNextSibling;
    }
}

} // namespace NE
#endif // USE_HEAP_TRACKING


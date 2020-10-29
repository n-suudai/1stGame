
#include "HeapFactory.hpp"

#if USE_HEAP_TRACKING

#include "HeapWalk.hpp"
#include <assert.h>

namespace NE
{

HeapFactory& HeapFactory::Get()
{
    static HeapFactory instance;
    return instance;
}

HeapFactory::HeapFactory()
{
    Initialize();
}

HeapFactory::~HeapFactory()
{
}

void HeapFactory::Initialize()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        heap.Initialize();
    }

    m_pRootHeap = CreateNewHeap(s_pRootHeapName);
    m_pDefaultHeap = CreateHeap(s_pDefaultHeapName);
}

Heap* HeapFactory::GetRootHeap()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        Initialize();
    }

    return m_pRootHeap;
}

Heap* HeapFactory::GetDefaultHeap()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (m_pDefaultHeap == nullptr)
    {
        Initialize();
    }

    return m_pDefaultHeap;
}

Heap* HeapFactory::CreateHeap(const char* name, const char* parentName)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        Initialize();
    }

    // 親ヒープを探す
    Heap* pParent = FindHeap(parentName);
    if (pParent == nullptr)
    { // 見つからなければ Root 以下に作成
        pParent = CreateNewHeap(parentName);
        pParent->AttachTo(m_pRootHeap);
    }

    // ヒープを探す
    Heap* pHeap = FindHeap(name);
    if (pHeap == nullptr)
    { // 見つからなければ作成
        pHeap = CreateNewHeap(name);
    }

    // pParent以下にアタッチ
    pHeap->AttachTo(pParent);

    return pHeap;
}

Heap* HeapFactory::CreateHeap(const char* name)
{
    return CreateHeap(name, s_pRootHeapName);
}

Heap* HeapFactory::FindHeap(const char* name)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive() && !_stricmp(name, heap.GetName()))
        {
            return &heap;
        }
    }

    return nullptr;
}

Heap* HeapFactory::CreateNewHeap(const char* name)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (!heap.IsActive())
        {
            heap.Activate(name);
            return &heap;
        }
    }

    assert(false &&
           "ヒープの作成に失敗しました。作成可能なヒープの数を超えています。");
    return nullptr;
}

// リークのチェック関数
void HeapFactory::MemoryLeakCheck(IMemoryLeakReporter* pReporter,
                                  SizeT bookmarkStart, SizeT bookmarkEnd) const
{
    assert(pReporter != nullptr);

    pReporter->BeginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive())
        {
            heap.MemoryLeakCheck(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->EndReport();
}

void HeapFactory::MemoryLeakCheck(IMemoryLeakReporter* pReporter,
                                  SizeT bookmarkStart)
{
    MemoryLeakCheck(pReporter, bookmarkStart,
                    MemoryTracker::Get().GetAllocationBookmark());
}

// メモリ情報
void HeapFactory::ReportHeapTreeStats(IHeapTreeStatsReporter* pReporter)
{
    assert(pReporter != nullptr);

    pReporter->BeginReport();

    m_pRootHeap->ReportTreeStats(pReporter, 0);

    pReporter->EndReport();
}

// メモリ破壊のチェック関数
void HeapFactory::MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                                       SizeT bookmarkStart,
                                       SizeT bookmarkEnd) const
{
    assert(pReporter != nullptr);

    pReporter->BeginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive())
        {
            heap.MemoryAssertionCheck(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->EndReport();
}

void HeapFactory::MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                                       SizeT bookmarkStart) const
{
    MemoryAssertionCheck(pReporter, bookmarkStart,
                         MemoryTracker::Get().GetAllocationBookmark());
}

} // namespace NE

#endif // USE_HEAP_TRACKING


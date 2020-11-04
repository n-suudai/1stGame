
#pragma once

#include "Heap.hpp"

#if USE_HEAP_TRACKING

namespace NE
{

class HeapFactory
{
public:
    static HeapFactory& Get();

    HeapFactory();

    ~HeapFactory();

    void Initialize();

    Heap* GetRootHeap();

    Heap* GetDefaultHeap();

    Heap* CreateHeap(const char* name, const char* parentName);

    Heap* CreateHeap(const char* name);

    Heap* FindHeap(const char* name);

    Heap* CreateNewHeap(const char* name);

    // リークのチェック関数
    void MemoryLeakCheck(IMemoryLeakReporter* pReporter, SizeT bookmarkStart,
                         SizeT bookmarkEnd) const;

    void MemoryLeakCheck(IMemoryLeakReporter* pReporter,
                         SizeT bookmarkStart = 0);

    // メモリ情報
    void ReportHeapTreeStats(IHeapTreeStatsReporter* pReporter);

    // メモリ破壊のチェック関数
    void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                              SizeT bookmarkStart, SizeT bookmarkEnd) const;

    void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                              SizeT bookmarkStart = 0) const;

private:
    static constexpr char s_pRootHeapName[] = "Root";
    static constexpr char s_pDefaultHeapName[] = "Default";

    std::recursive_mutex m_protection;

    enum
    {
        MAXHEAPS = 256
    };
    Heap m_heaps[MAXHEAPS];
    Heap* m_pRootHeap;
    Heap* m_pDefaultHeap;
};

} // namespace NE

#endif // USE_HEAP_TRACKING

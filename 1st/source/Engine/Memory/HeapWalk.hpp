
#pragma once

#include "MemoryConfig.hpp"

#if USE_HEAP_TRACKING

namespace NE
{

class Heap;

struct Allocation;

// メモリリーク
class IMemoryLeakReporter
{
public:
    virtual ~IMemoryLeakReporter() = default;

    virtual void BeginReport() = 0;

    virtual void Report(const Heap* pHeap, const Allocation* pAllocation) = 0;

    virtual void EndReport() = 0;
};

// メモリ情報
class IHeapTreeStatsReporter
{
public:
    struct HeapTreeStats
    {
        SizeT totalBytes;
        SizeT peakBytes;
        SizeT instanceCount;

        inline HeapTreeStats() : totalBytes(0), peakBytes(0), instanceCount(0)
        {
        }
    };

    virtual ~IHeapTreeStatsReporter() = default;

    virtual void BeginReport() = 0;

    virtual void Report(S32 depth, const Heap* pHeap,
                        const HeapTreeStats& local,
                        const HeapTreeStats& total) = 0;

    virtual void EndReport() = 0;
};

// メモリ破壊
class IMemoryAssertionReporter
{
public:
    virtual ~IMemoryAssertionReporter() = default;

    virtual void BeginReport() = 0;

    virtual void Report(const Heap* pHeap, const Allocation* pAllocation) = 0;

    virtual void EndReport() = 0;
};

} // namespace NE

#endif // USE_HEAP_TRACKING

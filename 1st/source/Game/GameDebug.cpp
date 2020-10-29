

#include "../Engine/Memory/Heap.hpp"
#include "../Engine/Memory/HeapFactory.hpp"
#include "../Engine/Memory/HeapWalk.hpp"
#include <assert.h>
#include <stdio.h>
#include <iostream>

#define DebugPrintf(format, ...)              \
    {                                         \
        char buff[256];                       \
        sprintf_s(buff, format, __VA_ARGS__); \
        printf_s(buff);                       \
    }

#if USE_HEAP_TRACKING

class TreeStatsReporter : public NE::IHeapTreeStatsReporter
{
public:
    TreeStatsReporter()
    {
    }
    ~TreeStatsReporter()
    {
    }

    void BeginReport() override
    {
        DebugPrintf("MEMORY INFORMATION\n");
        DebugPrintf(
          "                            Local                 Total\n");
        DebugPrintf(
          "Name                  Memory  Peak  Inst   Memory  Peak  Inst\n");
    }

    void Report(int depth, const NE::Heap* pHeap, const HeapTreeStats& local,
                const HeapTreeStats& total) override
    {
        for (int i = 0; i < depth; ++i)
        {
            DebugPrintf("..");
        }

        int spacing = 20 - depth * 2;
        DebugPrintf("%-*s %6zu %6zu %5zu  %6zu %6zu %5zu\n", spacing,
                    pHeap->GetName(), local.totalBytes, local.peakBytes,
                    local.instanceCount, total.totalBytes, total.peakBytes,
                    total.instanceCount);
    }

    void EndReport() override
    {
    }
};

class LeakReporter : public NE::IMemoryLeakReporter
{
public:
    LeakReporter() : m_leakCount(0)
    {
    }
    ~LeakReporter()
    {
    }

    void BeginReport() override
    {
        m_leakCount = 0;
    }

    void Report(const NE::Heap* pHeap, const NE::Allocation* pAllocation)
      override
    {
        assert(pHeap != nullptr);
        assert(pAllocation != nullptr);

        DebugPrintf(
          "%s(%d)\n"
          "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
          "backTraceHash=0x%016llX bookmark=%llX }\n"
          "[ %08X ]\n",
          pAllocation->file, pAllocation->line, pHeap->GetName(),
          pAllocation->pBlock, pAllocation->bytes,
          /*pAllocation->time*/ "", pAllocation->backTraceHash,
          pAllocation->bookmark, (*pAllocation->pSignature));

        m_leakCount++;
    }

    void EndReport() override
    {
        if (m_leakCount > 0)
        {
            DebugPrintf("%zu memory leaks found!\n", m_leakCount);
        }
        else
        {
            DebugPrintf("No memory leaks detected.\n");
        }
    }

public:
    size_t m_leakCount;
};

class AssertionReporter : public NE::IMemoryAssertionReporter
{
public:
    AssertionReporter() : m_assertionCount(0)
    {
    }
    ~AssertionReporter()
    {
    }

    void BeginReport()
    {
        m_assertionCount = 0;
    }

    void Report(const NE::Heap* pHeap, const NE::Allocation* pAllocation)
    {
        assert(pHeap != nullptr);
        assert(pAllocation != nullptr);

        DebugPrintf(
          "%s(%d)\n"
          "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
          "backTraceHash=0x%016llX bookmark=%llX }\n"
          "[ %08X ]\n",
          pAllocation->file, pAllocation->line, pHeap->GetName(),
          pAllocation->pBlock, pAllocation->bytes,
          /*pAllocation->time*/ "", pAllocation->backTraceHash,
          pAllocation->bookmark, (*pAllocation->pSignature));

        m_assertionCount++;
    }

    void EndReport()
    {
        if (m_assertionCount > 0)
        {
            DebugPrintf("%zu memory assertions occurred!\n", m_assertionCount);
        }
        else
        {
            DebugPrintf("No memory assertions detected.\n");
        }
    }

public:
    size_t m_assertionCount;
};

void PrintDebug_Report_HeapTreeStats()
{
    TreeStatsReporter statsReporter;
    NE::HeapFactory::Get().ReportHeapTreeStats(&statsReporter);
}
void PrintDebug_Report_MemoryLeaks()
{
    LeakReporter leakReporter;
    NE::HeapFactory::Get().MemoryLeakCheck(&leakReporter);
}
void PrintDebug_Report_MemoryAssertions()
{
    AssertionReporter assertionReporter;
    NE::HeapFactory::Get().MemoryAssertionCheck(&assertionReporter);
}

void PrintDebug_Report_MemoryAll()
{
    PrintDebug_Report_HeapTreeStats();
    PrintDebug_Report_MemoryLeaks();
    PrintDebug_Report_MemoryAssertions();
}

#else // !USE_HEAP_TRACKING

void PrintDebug_Report_HeapTreeStats()
{
    DebugPrintf("HeapTracking is not enabled.\n");
}
void PrintDebug_Report_MemoryLeaks()
{
    DebugPrintf("HeapTracking is not enabled.\n");
}
void PrintDebug_Report_MemoryAssertions()
{
    DebugPrintf("HeapTracking is not enabled.\n");
}

void PrintDebug_Report_MemoryAll()
{
    PrintDebug_Report_HeapTreeStats();
    PrintDebug_Report_MemoryLeaks();
    PrintDebug_Report_MemoryAssertions();
}

#endif // USE_HEAP_TRACKING
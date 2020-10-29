
#pragma once

#include "MemoryConfig.hpp"

#if USE_HEAP_TRACKING

#include "STL/HeapAllocator_NonTracking.hpp"
#include <map>
#include <mutex>

namespace NE
{

class Heap;

struct Allocation;

class MemoryTracker
{
public:
    typedef typename std::map<
      void*, Allocation*, std::less<void*>,
      STL::SHeapAllocator_NonTracking<std::pair<const void*, Allocation*>>> TrackMap;

    static MemoryTracker& Get();

    MemoryTracker();

    ~MemoryTracker();

    Allocation* RecordAllocation(void* pBlock, SizeT bytes, const char* file,
                                 S32 line, const char* function, Heap* pHeap);

    void RecordDeallocation(void* pBlock, Heap* pHeap);

    SizeT GetAllocationBookmark() const;

private:
    std::recursive_mutex m_protection;
    TrackMap m_allocations;
    SizeT m_nextAllocationBookmark;
};

} // namespace NE

#endif // USE_HEAP_TRACKING


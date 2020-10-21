
#pragma once

#include "../Types.hpp"
#include <mutex>
#include <unordered_map>

namespace NE
{

class Heap;

struct Allocation;

class MemoryTracker
{
public:
    static MemoryTracker& Get();

    MemoryTracker();

    ~MemoryTracker();

    Allocation* RecordAllocation(void* pBlock, SizeT bytes, const char* file,
                                 S32 line, const char* function, Heap* pHeap);

    void RecordDeallocation(void* pBlock, Heap* pHeap);

    SizeT GetAllocationBookmark() const;

private:
    std::recursive_mutex m_protection;
    std::unordered_map<void*, Allocation*> m_allocations;
    SizeT m_nextAllocationBookmark;
};

} // namespace NE

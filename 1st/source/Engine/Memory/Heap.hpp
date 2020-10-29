
#pragma once

#include "MemoryTracker.hpp"

#if USE_HEAP_TRACKING

#include <mutex>

namespace NE
{

class IMemoryLeakReporter;
class IHeapTreeStatsReporter;
class IMemoryAssertionReporter;

typedef S32 AllocationSignature;

struct Allocation
{
    static constexpr AllocationSignature SIGNATURE = 0xEFEFEFEF;

    void* pBlock;                    // アドレス
    SizeT bytes;                     // 確保サイズ
    U32 pool;                        // 確保したメモリプール
    const char* file;                // ファイル名
    S32 line;                        // 行数
    const char* function;            // 関数名
    time_t time;                     // 確保日時
    SizeT backTraceHash;             // バックトレースのハッシュ値
    AllocationSignature* pSignature; // メモリ破壊チェック用
    SizeT bookmark;                  // ブックマーク
    Heap* pHeap;                     // 確保したヒープ領域

    Allocation* pNext; // リンクリスト (ヒープをウォークするのに必要)
    Allocation* pPrev;

    static void* operator new(NE::SizeT bytes);
    static void operator delete(void* pBlock);
};

// ヒープ
class Heap
{
public:
    Heap();

    ~Heap();

    void Initialize();

    void Activate(const char* name);

    void Deactivate();

    const char* GetName() const;

    bool IsActive() const;

    template <typename Policy>
    inline void* Allocate(SizeT bytes, const char* file, S32 line,
                          const char* function);

    template <typename Policy>
    inline void Deallocate(void* pBlock);

    // リンクリストを構築
    void AddAllocation(Allocation* pAllocation);

    // リンクリストから切り離す
    void EraseAllocation(Allocation* pAllocation);

    // 親子関係の構築をする関数
    void AttachTo(Heap* pParent);

    // リークのチェック関数
    void MemoryLeakCheck(IMemoryLeakReporter* pReporter, SizeT bookmarkStart,
                         SizeT bookmarkEnd) const;

    // 情報収集のための関数
    void ReportTreeStats(IHeapTreeStatsReporter* pAccumulator, S32 depth) const;

    // メモリ破壊のチェック関数
    void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                              SizeT bookmarkStart, SizeT bookmarkEnd) const;

protected:
    void GetTreeStats(SizeT& totalBytes, SizeT& totalPeakBytes,
                      SizeT& totalInstanceCount) const;

private:
    std::recursive_mutex m_protection;

    enum
    {
        NAMELENGTH = 128
    };
    char m_name[NAMELENGTH];

    SizeT m_totalAllocatedBytes;
    SizeT m_peakAllocatedBytes;
    SizeT m_allocatedInstanceCount;
    Allocation* m_pAllocation; // リンクリスト

    Heap* m_pParent;
    Heap* m_pFirstChild;
    Heap* m_pNextSibling;
    Heap* m_pPrevSibling;

    bool m_isActive;
};

template <typename Policy>
inline void* Heap::Allocate(size_t bytes, const char* file, S32 line,
                            const char* function)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    // シグネチャサイズをプラス
    constexpr size_t signatureSize = sizeof(AllocationSignature);

    // ポリシーを利用してメモリを確保
    void* pBlock = Policy::AllocateBytes(bytes + signatureSize);

    // トラッカーへ情報を登録
    MemoryTracker::Get().RecordAllocation(pBlock, bytes, file, line, function,
                                          this);

    return pBlock;
}

template <typename Policy>
inline void Heap::Deallocate(void* pBlock)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    // トラッカーから情報を削除
    MemoryTracker::Get().RecordDeallocation(pBlock, this);

    // ポリシーを利用してメモリを破棄
    Policy::DeallocateBytes(pBlock);
}

class HeapAllocatorBase
{
public:
    virtual ~HeapAllocatorBase() = default;

    virtual void* AllocateBytes(SizeT bytes) = 0;

    virtual void* AllocateAlignedBytes(SizeT bytes, SizeT alignment) = 0;

    virtual void DeallocateBytes(void* pBlock) = 0;

    virtual void DeallocateAlignedBytes(void* pBlock) = 0;
};

template <typename Policy>
class HeapAllocatorOverride : public HeapAllocatorBase
{
public:
    static HeapAllocatorBase* Get()
    {
        static HeapAllocatorOverride instance;
        return &instance;
    }

    inline void* AllocateBytes(SizeT bytes) override
    {
        return Policy::AllocateBytes(bytes);
    }

    inline void* AllocateAlignedBytes(SizeT bytes, SizeT alignment) override
    {
        return Policy::AllocateAlignedBytes(bytes, alignment);
    }

    inline void DeallocateBytes(void* pBlock) override
    {
        Policy::DeallocateBytes(pBlock);
    }

    inline void DeallocateAlignedBytes(void* pBlock) override
    {
        Policy::DeallocateAlignedBytes(pBlock);
    }
};

class HeapCustom
{
public:
    template <typename Policy>
    inline void Activate(const char* name)
    {
        m_pAllocator = HeapAllocatorOverride<Policy>::Get();
    }

private:
    HeapAllocatorBase* m_pAllocator;
};

} // namespace NE

#endif // USE_HEAP_TRACKING


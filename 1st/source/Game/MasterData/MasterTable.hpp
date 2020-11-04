#pragma once

#include "../../Engine/STL.hpp"
#include "../Entities.hpp"
#include "MasterValueType.hpp"
#include <functional>

// メモリ使用量
// 検索速度

class IMasterParser;
class MasterRecord;

// テーブル
class MasterTable : public MasterEntity
{
public:
    struct ValueTypeAndOffset
    {
        MasterValueType type;
        NE::SizeT offset;
    };

    typedef NE::UnorderedMap<NE::String, ValueTypeAndOffset> ColumnDefineList;

    typedef std::function<void(const MasterRecord*)> RecordAccessor;

public:
    MasterTable(const char* pFilename,
                NE::SharedPtr<IMasterParser> parser = nullptr);
    ~MasterTable();

    // Parser から呼び出し
    void SetColumnDefines(const ColumnDefineList& defines);
    void SetRecordBlockSize(NE::SizeT size);
    void SetRecordCount(NE::SizeT count);
    void SetTableBlock(NE::UniqueBlob tableBlock);

    const ColumnDefineList& GetColumnDefines() const;
    NE::U64 GetRecordBlockSize() const;
    NE::U64 GetRecordCount() const;
    NE::U8* GetTableBlockTop() const;

    NE::SizeT GetColumnOffset(const NE::String& columnName) const;
    NE::U8* GetColumnAddr(const NE::String& columnName, NE::SizeT index) const;
    NE::U8* GetColumnAddr(NE::SizeT offset, NE::SizeT index) const;

    void ForEach(const RecordAccessor& accessor) const;

    void DebugPrint(bool columnNames = true);

private:
    ColumnDefineList m_columnDefines;
    NE::SizeT m_recordBlockSize; // Stride
    NE::SizeT m_recordCount;
    NE::UniqueBlob m_tableBlock;
    NE::SharedPtr<IMasterParser> m_parser;
};

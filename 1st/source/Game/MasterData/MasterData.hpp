#pragma once

#include "../Entities.hpp"
#include "../../Engine/Types.hpp"
#include "../../Engine/STL.hpp"
#include <functional>


enum class MasterValueType
{
    S8,
    S16,
    S32,
    S64,

    U8,
    U16,
    U32,
    U64,

    Float,
    Double,

    String,
};

inline NE::SizeT GetValueSize(MasterValueType type)
{
    switch (type)
    {
        case MasterValueType::S8:
        case MasterValueType::S16:
        case MasterValueType::S32:
        case MasterValueType::U8:
        case MasterValueType::U16:
        case MasterValueType::U32:
            return sizeof(NE::S32);

        case MasterValueType::S64:
        case MasterValueType::U64:
            return sizeof(NE::S64);

        case MasterValueType::Float:
            return sizeof(NE::Float);

        case MasterValueType::Double:
            return sizeof(NE::Double);

        case MasterValueType::String:
            return sizeof(NE::U64);

        default:
            break;
    }

    return 0;
}


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
    MasterTable(const char* pFilename, NE::SharedPtr<IMasterParser> parser = nullptr);
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

// レコード
class MasterRecord : public MasterEntity
{
public:
    MasterRecord(const MasterTable& owner);

    ~MasterRecord();

    void SetIndex(NE::SizeT index);

    NE::S8 Get_S8(const NE::String& columnName) const;
    NE::S16 Get_S16(const NE::String& columnName) const;
    NE::S32 Get_S32(const NE::String& columnName) const;
    NE::S64 Get_S64(const NE::String& columnName) const;

    NE::U8 Get_U8(const NE::String& columnName) const;
    NE::U16 Get_U16(const NE::String& columnName) const;
    NE::U32 Get_U32(const NE::String& columnName) const;
    NE::U64 Get_U64(const NE::String& columnName) const;

    NE::Float Get_Float(const NE::String& columnName) const;
    NE::Double Get_Double(const NE::String& columnName) const;

    NE::String Get_String(const NE::String& columnName) const;

private:
    const MasterTable& m_owner;
    NE::SizeT m_index;
};


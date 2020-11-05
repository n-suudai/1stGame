#include "MasterTable.hpp"
#include "MasterRecord.hpp"
#include "Parser/IMasterParser.hpp"

// テーブル
MasterTable::MasterTable(const char* pFilename,
                         NE::SharedPtr<IMasterParser> parser)
    : m_columnDefines()
    , m_recordBlockSize(0)
    , m_recordCount(0)
    , m_tableBlock(nullptr)
    , m_parser(parser)
{
    if (m_parser)
    {
        m_parser->Parse(pFilename, this);
    }
}

MasterTable::~MasterTable()
{
    if (m_parser)
    {
        m_parser->Free(this);
    }
}

// Parser から呼び出し
void MasterTable::SetColumnDefines(const ColumnDefineList& defines)
{
    m_columnDefines = defines;
}

void MasterTable::SetRecordBlockSize(NE::SizeT size)
{
    m_recordBlockSize = size;
}

void MasterTable::SetRecordCount(NE::SizeT count)
{
    m_recordCount = count;
}

void MasterTable::SetTableBlock(NE::UniqueBlob tableBlock)
{
    m_tableBlock = std::move(tableBlock);
}

const MasterTable::ColumnDefineList& MasterTable::GetColumnDefines() const
{
    return m_columnDefines;
}

NE::U64 MasterTable::GetRecordBlockSize() const
{
    return m_recordBlockSize;
}

NE::U64 MasterTable::GetRecordCount() const
{
    return m_recordCount;
}

NE::U8* MasterTable::GetTableBlockTop() const
{
    return m_tableBlock.get();
}

NE::SizeT MasterTable::GetColumnOffset(const NE::String& columnName) const
{
    auto it = m_columnDefines.find(columnName);
    if (it != m_columnDefines.end())
    {
        return it->second.offset;
    }
    return 0;
}

NE::U8* MasterTable::GetColumnAddr(const NE::String& columnName,
                                   NE::SizeT index) const
{
    return GetColumnAddr(GetColumnOffset(columnName), index);
}

NE::U8* MasterTable::GetColumnAddr(NE::SizeT offset, NE::SizeT index) const
{
    offset = index * m_recordBlockSize + offset;

    if (offset < 0 || offset >= (m_recordBlockSize * m_recordCount))
    {
        return nullptr;
    }

    return m_tableBlock.get() + offset;
}

void MasterTable::ForEach(const RecordAccessor& accessor) const
{
    MasterRecord record(*this);

    for (NE::SizeT index = 0; index < m_recordCount; index++)
    {
        record.SetIndex(index);
        accessor(&record);
    }
}

void MasterTable::DebugPrint(bool columnNames)
{

    if (columnNames)
    {
        NE::StringStream ss;

        for (auto& it : m_columnDefines)
        {
            ss << it.first << "\t";
        }

        printf_s("%s\n", ss.str().c_str());
    }

    for (NE::SizeT record = 0; record < m_recordCount; record++)
    {
        NE::StringStream ss;

        for (auto& it : m_columnDefines)
        {
            NE::U8* addr = GetColumnAddr(it.second.offset, record);

            it.second.type.GetValueString(addr, ss);

            ss << "\t";
        }

        printf_s("%s\n", ss.str().c_str());
    }
}

#include "MasterData.hpp"
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

template <typename AddressType, typename Type>
static void GetIntegerString(NE::U8* addr, NE::StringStream& ss)
{
    Type value = static_cast<Type>(*reinterpret_cast<AddressType*>(addr));
    ss << value;
}

template <typename Type>
static void GetFloatingPointString(NE::U8* addr, NE::StringStream& ss)
{
    Type value = (*reinterpret_cast<Type*>(addr));
    ss << value;
}

static void GetStringString(NE::U8* addr, NE::StringStream& ss)
{
    NE::U64 stringAddr = (*reinterpret_cast<NE::U64*>(addr));
    ss << NE::String(reinterpret_cast<const char*>(stringAddr));
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

            switch (it.second.type)
            {
                case MasterValueType::S8:
                    GetIntegerString<NE::S32, NE::S8>(addr, ss);
                    break;

                case MasterValueType::S16:
                    GetIntegerString<NE::S32, NE::S16>(addr, ss);
                    break;

                case MasterValueType::S32:
                    GetIntegerString<NE::S32, NE::S32>(addr, ss);
                    break;

                case MasterValueType::S64:
                    GetIntegerString<NE::S64, NE::S64>(addr, ss);
                    break;

                case MasterValueType::U8:
                    GetIntegerString<NE::U32, NE::U8>(addr, ss);
                    break;

                case MasterValueType::U16:
                    GetIntegerString<NE::U32, NE::U16>(addr, ss);
                    break;

                case MasterValueType::U32:
                    GetIntegerString<NE::U32, NE::U32>(addr, ss);
                    break;

                case MasterValueType::U64:
                    GetIntegerString<NE::U64, NE::U64>(addr, ss);
                    break;

                case MasterValueType::Float:
                    GetFloatingPointString<NE::Float>(addr, ss);
                    break;

                case MasterValueType::Double:
                    GetFloatingPointString<NE::Double>(addr, ss);
                    break;

                case MasterValueType::String:
                    GetStringString(addr, ss);
                    break;
            }

            ss << "\t";
        }

        printf_s("%s\n", ss.str().c_str());
    }
}

// レコード
MasterRecord::MasterRecord(const MasterTable& owner)
    : m_owner(owner), m_index(0)
{
}

MasterRecord::~MasterRecord()
{
}

void MasterRecord::SetIndex(NE::SizeT index)
{
    m_index = index;
}

NE::S8 MasterRecord::Get_S8(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::S8*>(addr));
}

NE::S16 MasterRecord::Get_S16(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::S16*>(addr));
}

NE::S32 MasterRecord::Get_S32(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::S32*>(addr));
}

NE::S64 MasterRecord::Get_S64(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::S64*>(addr));
}

NE::U8 MasterRecord::Get_U8(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::U8*>(addr));
}

NE::U16 MasterRecord::Get_U16(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::U16*>(addr));
}

NE::U32 MasterRecord::Get_U32(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::U32*>(addr));
}

NE::U64 MasterRecord::Get_U64(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::U64*>(addr));
}

NE::Float MasterRecord::Get_Float(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::Float*>(addr));
}

NE::Double MasterRecord::Get_Double(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    return (*reinterpret_cast<NE::Double*>(addr));
}

NE::String MasterRecord::Get_String(const NE::String& columnName) const
{
    NE::U8* addr = m_owner.GetColumnAddr(columnName, m_index);
    NE::U64 stringAddr = (*reinterpret_cast<NE::U64*>(addr));
    return NE::String(reinterpret_cast<char*>(stringAddr));
}

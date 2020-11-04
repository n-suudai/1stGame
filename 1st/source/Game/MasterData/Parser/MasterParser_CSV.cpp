#include "MasterParser_CSV.hpp"
#include "../MasterTable.hpp"
#include <array>
#include <fstream>

void GetStringList(const NE::String& text, NE::String::value_type delimiter,
                   NE::Vector<NE::String>& stringList)
{
    stringList.clear();

    NE::StringStream ss(text);
    NE::String element;

    while (std::getline(ss, element, delimiter))
    {
        stringList.push_back(element);
    }
}

MasterValueType GetValueType(const NE::String& typeText)
{
    static const char* s_pValueTypeTexts
      [static_cast<NE::S32>(MasterValueType::Num)] = {
        "S8",  "S16", "S32",   "S64",    "U8",     "U16",
        "U32", "U64", "Float", "Double", "String", };

    for (NE::S32 type = static_cast<NE::S32>(MasterValueType::S8);
         type < static_cast<NE::S32>(MasterValueType::Num); type++)
    {
        if (typeText == s_pValueTypeTexts[type])
        {
            return static_cast<MasterValueType>(type);
        }
    }

    return MasterValueType::ErrorType;
}

NE::SizeT MakeColumnDefineList(const NE::String& line1, const NE::String& line2,
                               MasterTable::ColumnDefineList& columnDefineList)
{
    columnDefineList.clear();

    NE::Vector<NE::String> columnNames;
    NE::Vector<NE::String> columnTypes;

    // 1行目 カラム名
    GetStringList(line1, ',', columnNames);

    // 2行目 カラム型名
    GetStringList(line2, ',', columnTypes);

    NE::SizeT columnCount = columnNames.size();

    if (columnCount > columnTypes.size())
    {
        columnCount = columnTypes.size();
    }

    NE::SizeT currentOffset = 0;
    bool result = true;

    for (NE::SizeT i = 0; i < columnCount; i++)
    {
        auto it = columnDefineList.find(columnNames[i]);

        if (it != columnDefineList.end()) // カラム名被り
        {
            result = false;
            break;
        }

        MasterTable::ValueTypeAndOffset& typeAndOffset =
          columnDefineList[columnNames[i]];

        typeAndOffset.type = GetValueType(columnTypes[i]);

        if (typeAndOffset.type == MasterValueType::ErrorType)
        {
            result = false;
            break;
        }

        typeAndOffset.offset = currentOffset;

        currentOffset += GetValueSize(typeAndOffset.type);
    }

    if (!result)
    {
        currentOffset = 0;
        columnDefineList.clear();
    }

    return currentOffset;
}

template <class T>
T ToValue(const NE::String& valueStr, T notFound = T())
{
    if (valueStr == "")
    {
        return notFound;
    }

    NE::IStringStream iss;

    iss.str(valueStr);

    T value;

    iss >> value;

    return value;
}

template <typename T>
NE::U8* WriteValue(NE::U8* pBlock, const NE::String& valueStr)
{
    T value = ToValue<T>(valueStr, T(0));

    T* p = reinterpret_cast<T*>(pBlock);

    (*p) = value;

    return pBlock + sizeof(T);
}

template <>
NE::U8* WriteValue<NE::String>(NE::U8* pBlock, const NE::String& valueStr)
{
    NE::SizeT length = strlen(valueStr.c_str());

    void* pMem = NE_MEM_ALLOC(length + 1);

    memcpy_s(pMem, length + 1, valueStr.c_str(), length);

    NE::U64& addr = (*reinterpret_cast<NE::U64*>(pBlock));

    addr = reinterpret_cast<NE::U64>(pMem);

    return pBlock + sizeof(NE::U64);
}

MasterParser_CSV::MasterParser_CSV()
{
}

MasterParser_CSV::~MasterParser_CSV()
{
}

bool MasterParser_CSV::Parse(const char* pFilename, MasterTable* pOwner)
{
    std::ifstream ifs(pFilename);
    MasterTable::ColumnDefineList columnDefines;
    NE::SizeT recordBlockSize = 0;

    {
        NE::String line1;
        if (!std::getline(ifs, line1))
        {
            return false;
        }

        NE::String line2;
        if (!std::getline(ifs, line2))
        {
            return false;
        }

        recordBlockSize = MakeColumnDefineList(line1, line2, columnDefines);

        if (recordBlockSize == 0)
        {
            return false;
        }
    }

    NE::String line;
    NE::Vector<NE::Vector<NE::String>> lineValues;

    while (std::getline(ifs, line))
    {
        if (line == "")
        {
            break;
        }

        NE::Vector<NE::String> values;

        GetStringList(line, ',', values);

        if (columnDefines.size() != values.size())
        {
            return false;
        }

        lineValues.push_back(std::move(values));
    }

    NE::UniqueBlob tableBlock =
      MAKE_UNIQUE_BLOB(lineValues.size() * recordBlockSize);

    for (NE::SizeT lineIndex = 0; lineIndex < lineValues.size(); lineIndex++)
    {
        NE::Vector<NE::String>& values = lineValues[lineIndex];
        NE::U8* pCurrentBlock = tableBlock.get() + recordBlockSize * lineIndex;

        NE::SizeT columnIndex = 0;
        for (auto column : columnDefines)
        {

            switch (column.second.type)
            {
                case MasterValueType::S8:
                case MasterValueType::S16:
                case MasterValueType::S32:
                    pCurrentBlock =
                      WriteValue<NE::S32>(pCurrentBlock, values[columnIndex]);
                    break;

                case MasterValueType::U8:
                case MasterValueType::U16:
                case MasterValueType::U32:
                    pCurrentBlock =
                      WriteValue<NE::U32>(pCurrentBlock, values[columnIndex]);
                    break;

                case MasterValueType::S64:
                    pCurrentBlock =
                      WriteValue<NE::S64>(pCurrentBlock, values[columnIndex]);
                    break;

                case MasterValueType::U64:
                    pCurrentBlock =
                      WriteValue<NE::U64>(pCurrentBlock, values[columnIndex]);
                    break;

                case MasterValueType::Float:
                    pCurrentBlock =
                      WriteValue<NE::Float>(pCurrentBlock, values[columnIndex]);
                    break;

                case MasterValueType::Double:
                    pCurrentBlock = WriteValue<NE::Double>(pCurrentBlock,
                                                           values[columnIndex]);
                    break;

                case MasterValueType::String:
                    pCurrentBlock = WriteValue<NE::String>(pCurrentBlock,
                                                           values[columnIndex]);
                    break;

                default:
                    break;
            }
            columnIndex++;
        }
    }

    pOwner->SetColumnDefines(columnDefines);

    pOwner->SetRecordBlockSize(recordBlockSize);

    pOwner->SetRecordCount(lineValues.size());

    pOwner->SetTableBlock(std::move(tableBlock));

    return true;
}

bool MasterParser_CSV::Free(MasterTable* pOwner)
{
    const MasterTable::ColumnDefineList& columnDefines =
      pOwner->GetColumnDefines();
    NE::SizeT recordBlockSize = pOwner->GetRecordBlockSize();
    NE::SizeT recordCount = pOwner->GetRecordCount();
    NE::U8* pTableBlockTop = pOwner->GetTableBlockTop();

    for (NE::SizeT record = 0; record < recordCount; record++)
    {
        NE::U8* pRecordTop = pTableBlockTop + (record * recordBlockSize);

        for (auto& it : columnDefines)
        {
            if (it.second.type == MasterValueType::String)
            {
                NE::U64& addr =
                  (*reinterpret_cast<NE::U64*>(pRecordTop + it.second.offset));

                void* pMem = reinterpret_cast<void*>(addr);

                NE_MEM_FREE(pMem);
            }
        }
    }

    return true;
}

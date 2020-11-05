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

        typeAndOffset.type = MasterValueType::FromString(columnTypes[i]);

        if (!typeAndOffset.type.Isvalid())
        {
            result = false;
            break;
        }

        typeAndOffset.offset = currentOffset;

        currentOffset += typeAndOffset.type.GetSize();
    }

    if (!result)
    {
        currentOffset = 0;
        columnDefineList.clear();
    }

    return currentOffset;
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
            pCurrentBlock = column.second.type.WriteMemoryBlock(
              pCurrentBlock, values[columnIndex]);
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
            it.second.type.FreeMemoryBlock(pRecordTop + it.second.offset);
        }
    }

    return true;
}

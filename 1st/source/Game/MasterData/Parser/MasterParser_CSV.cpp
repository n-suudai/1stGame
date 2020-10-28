#include "MasterParser_CSV.hpp"
#include "../MasterData.hpp"
#include <array>

void GetStringList(
    const NE::String& text,
    NE::String::value_type delimiter,
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



template<class T>
T ToValue(const NE::String& valueStr, T notFound = T())
{
    if (valueStr == "") { return notFound; }

    NE::IStringStream iss;

    iss.str(valueStr);

    T value;

    iss >> value;

    return value;
}


MasterValueType GetValueType(const NE::String& typeText)
{
    static const char* s_pValueTypeTexts[static_cast<NE::S32>(MasterValueType::Num)] = {
            "S8",
            "S16",
            "S32",
            "S64",
            "U8",
            "U16",
            "U32",
            "U64",
            "Float",
            "Double",
            "String",
    };

    for (NE::S32 type = static_cast<NE::S32>(MasterValueType::S8);
        type < static_cast<NE::S32>(MasterValueType::Num);
        type++)
    {
        if (typeText == s_pValueTypeTexts[type])
        {
            return static_cast<MasterValueType>(type);
        }
    }

    return MasterValueType::ErrorType;
}


bool MakeColumnDefineList(
    const NE::String& line1,
    const NE::String& line2,
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

        MasterTable::ValueTypeAndOffset& typeAndOffset = columnDefineList[columnNames[i]];
        
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
        columnDefineList.clear();
    }

    return result;
}



MasterParser_CSV::MasterParser_CSV()
{
}

MasterParser_CSV::~MasterParser_CSV()
{
}

bool MasterParser_CSV::Parse(const char*, MasterTable* pOwner)
{
    struct DummyData
    {
        NE::U64 id;
        NE::String name;
        NE::U64 age;
        NE::String className;
    };

    std::array<DummyData, 3> dummies = {
      DummyData{1000, "ボブ", 18, "人間"},
      DummyData{1001, "アリス", 25, "魔女"},
      DummyData{1003, "ドラゴ", 25, "ドラゴン"}, };

    NE::SizeT offset = 0;
    NE::SizeT recordCount = dummies.size();

    MasterTable::ColumnDefineList columnDefines;

    columnDefines.insert({"Id", {MasterValueType::U64, offset}});
    offset += GetValueSize(MasterValueType::U64);

    columnDefines.insert({"Name", {MasterValueType::String, offset}});
    offset += GetValueSize(MasterValueType::String);

    columnDefines.insert({"Age", {MasterValueType::U64, offset}});
    offset += GetValueSize(MasterValueType::U64);

    columnDefines.insert({"Class", {MasterValueType::String, offset}});
    offset += GetValueSize(MasterValueType::String);

    pOwner->SetColumnDefines(columnDefines);

    pOwner->SetRecordBlockSize(offset);

    pOwner->SetRecordCount(recordCount);

    NE::UniqueBlob tableBlock = MAKE_UNIQUE_BLOB(offset * recordCount);

    for (NE::SizeT record = 0; record < recordCount; record++)
    {
        NE::SizeT currentOffset = 0;
        NE::U8* pRecordTop = tableBlock.get() + (record * offset);

        // Id
        {
            NE::U64& id =
              (*reinterpret_cast<NE::U64*>(pRecordTop + currentOffset));
            currentOffset += sizeof(NE::U64);

            id = dummies[record].id;
        }

        // Name
        {
            NE::U64& addr =
              (*reinterpret_cast<NE::U64*>(pRecordTop + currentOffset));
            currentOffset += sizeof(NE::U64);

            NE::SizeT nameLength = strlen(dummies[record].name.c_str());

            void* pMem = MEM_ALLOC(nameLength + 1);

            memcpy_s(pMem, nameLength + 1, dummies[record].name.c_str(),
                     nameLength);

            addr = reinterpret_cast<NE::U64>(pMem);
        }

        // Age
        {
            NE::U64& age =
              (*reinterpret_cast<NE::U64*>(pRecordTop + currentOffset));
            currentOffset += sizeof(NE::U64);

            age = dummies[record].age;
        }

        // Class
        {
            NE::U64& addr =
              (*reinterpret_cast<NE::U64*>(pRecordTop + currentOffset));
            currentOffset += sizeof(NE::U64);

            NE::SizeT nameLength = strlen(dummies[record].className.c_str());

            void* pMem = MEM_ALLOC(nameLength + 1);

            memcpy_s(pMem, nameLength + 1, dummies[record].className.c_str(),
                     nameLength);

            addr = reinterpret_cast<NE::U64>(pMem);
        }
    }

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

                MEM_FREE(pMem);
            }
        }
    }

    return true;
}

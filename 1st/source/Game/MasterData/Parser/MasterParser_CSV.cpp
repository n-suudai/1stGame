#include "MasterParser_CSV.hpp"
#include "../MasterData.hpp"
#include <array>

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

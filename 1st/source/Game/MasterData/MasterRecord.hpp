#pragma once

#include "MasterTable.hpp"

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

#include "MasterRecord.hpp"

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

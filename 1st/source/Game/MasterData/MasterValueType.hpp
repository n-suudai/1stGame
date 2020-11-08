#pragma once

#include "../../Engine/STL.hpp"
#include "../Entities.hpp"

class MasterValueType : public MasterEntity
{
public:
    enum class Type
    {
        Error,
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
        Num,
    };

public:
    MasterValueType();

    explicit MasterValueType(Type type);

    NE::SizeT GetSize() const;

    NE::String ToString() const;

    static MasterValueType FromString(const NE::String& str);

    bool Isvalid() const;

    bool IsString() const;

    NE::U8* WriteMemoryBlock(NE::U8* pBlock, const NE::String& valueStr) const;

    void FreeMemoryBlock(NE::U8* pBlock) const;

    void GetValueString(NE::U8* pBlock, NE::StringStream& ss) const;

private:
    Type m_type;
};

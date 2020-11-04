#pragma once

#include "../../Engine/Types.hpp"

enum class MasterValueType : NE::S32
{
    ErrorType = -1,
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

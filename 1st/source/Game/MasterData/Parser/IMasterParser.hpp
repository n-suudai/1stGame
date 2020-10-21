#pragma once

#include "../../Entities.hpp"
#include "../../../Engine/Types.hpp"
#include "../../../Engine/STL.hpp"

class MasterTable;
class IMasterParser : public MasterEntity
{
public:
    virtual bool Parse(const char* pFilename, MasterTable* pOwner) = 0;

    virtual bool Free(MasterTable* pOwner) = 0;
};

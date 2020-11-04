#pragma once

#include "../../../Engine/STL.hpp"
#include "../../../Engine/Types.hpp"
#include "../../Entities.hpp"

class MasterTable;
class IMasterParser : public MasterEntity
{
public:
    virtual bool Parse(const char* pFilename, MasterTable* pOwner) = 0;

    virtual bool Free(MasterTable* pOwner) = 0;
};

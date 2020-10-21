#pragma once

#include "IMasterParser.hpp"


class MasterParser_CSV : public IMasterParser
{
public:
    MasterParser_CSV();
    ~MasterParser_CSV();

    bool Parse(const char* pFilename, MasterTable* pOwner) override;

    bool Free(MasterTable* pOwner) override;
};


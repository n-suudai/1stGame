#pragma once

#include "Memory/MemoryManager.hpp"

#define DEFINE_ENTITY_CLASS(className)  \
    class className                     \
    {                                   \
        DECLARE_HEAP(className);        \
                                        \
    protected:                          \
        className() = default;          \
        virtual ~className() = default; \
    };

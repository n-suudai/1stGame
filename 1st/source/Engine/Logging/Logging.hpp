#pragma once

#include "../Types.hpp"
#include "../STL.hpp"
#include "../Memory/MemoryManager.hpp"
#include <functional>

namespace NE
{
namespace Log
{
enum class Level
{
    Verbose,
    Debug,
    Warning,
    Error,
    Assert,
    Num
};

class Logger
{
    DECLARE_HEAP(Logger);

public:
    Logger();

    ~Logger() = default;

    void Print(const NE::String& str) const;

    NE::OStringStream& GetStream();

    void FlushStream();

protected:
    NE::OStringStream m_stream;
};

} // namespace Log

} // namespace NE

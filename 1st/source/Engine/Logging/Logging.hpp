#pragma once

#include "../STL.hpp"
#include "../Entity.hpp"
#include <functional>

namespace NE
{
namespace Log
{
DEFINE_ENTITY_CLASS(LoggingEntity);

class Level : public LoggingEntity
{
public:
    enum class Type
    {
        Verbose,
        Debug,
        Warning,
        Error,
        Assert,
        Num
    };

public:
    Level();

    explicit Level(Type type);

    ~Level() = default;

    NE::String ToString() const;

    static Level FromString(const NE::String& str);

private:
    Type m_type;
};

class Logger : public LoggingEntity
{
public:
    Logger();

    explicit Logger(const Level& level);

    ~Logger() = default;

    void Print(const NE::String& str) const;

    void ChangeLevel(const Level& level);

protected:
    Level m_level;
};

} // namespace Log

} // namespace NE

#include "Logging.hpp"
#include <array>

namespace NE
{
namespace Log
{
DEFINE_HEAP(LoggingEntity, "Log", AllocatePolicy);

Level::Level() : m_type(Type::Verbose)
{
}

Level::Level(Type type) : m_type(type)
{
}

NE::String Level::ToString() const
{
    constexpr std::array<const char*, static_cast<NE::SizeT>(Type::Num)>
    StringList = {"Verbose", "Debug", "Warning", "Error", "Assert", };

    return StringList[static_cast<NE::SizeT>(m_type)];
}

Level Level::FromString(const NE::String& str)
{
    static const std::array<Level, static_cast<NE::SizeT>(Type::Num)>
    Instances = {Level(Type::Verbose), Level(Type::Debug),
                 Level(Type::Warning), Level(Type::Error),
                 Level(Type::Assert), };

    auto it =
      std::find_if(Instances.begin(), Instances.end(), [str](const Level& value)
    { return value.ToString() == str; });

    if (it != Instances.end())
    {
        return *it;
    }

    return Level(Type::Verbose);
}

Logger::Logger() : Logger(Level(Level::Type::Verbose))
{
}

Logger::Logger(const Level& level) : m_level(level)
{
}

void Logger::Print(const NE::String& str) const
{
    printf_s("%s\n", str.c_str());
}

void Logger::ChangeLevel(const Level& level)
{
    m_level = level;
}

} // namespace Log
} // namespace NE
#pragma once

#include "Serialize.hpp"
#include "Entities.hpp"
#include <cmath>

template <typename T>
inline T Clamp(T x, T low, T high)
{
    assert(low <= high);
    return std::min(std::max(x, low), high);
}

template <typename T>
inline T Wrap(T x, T low, T high)
{
    assert(low < high);
    const T n = (x - low) % (high - low);
    return (n >= 0) ? (n + low) : (n + high);
}

// Float特殊化
template <>
inline NE::Float Wrap<NE::Float>(NE::Float x, NE::Float low, NE::Float high)
{
    assert(low < high);
    const NE::Float n = std::fmod(x - low, high - low);
    return (n >= 0) ? (n + low) : (n + high);
}

// Double特殊化
template <>
inline NE::Double Wrap<NE::Double>(NE::Double x, NE::Double low,
                                   NE::Double high)
{
    assert(low < high);
    const NE::Double n = std::fmod(x - low, high - low);
    return (n >= 0) ? (n + low) : (n + high);
}

// Range
template <typename T>
class Range : public PlayDataEntity
{
public:
    inline Range() : Range(T(0), T(1))
    {
    }

    inline Range(T min, T max) : m_min(min), m_max(max)
    {
        assert(min <= max);
    }

    inline bool IsInside(T x) const
    {
        return (m_min <= x) && (x <= m_max);
    }

    inline T Wrap(T x) const
    {
        return ::Wrap(x, m_min, m_max);
    }

    inline T Clamp(T x) const
    {
        return ::Clamp(x, m_min, m_max);
    }

    inline T Min() const
    {
        return m_min;
    }

    inline T Max() const
    {
        return m_max;
    }

    // シリアライズ可能
    template <typename Archive>
    inline void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Min", m_min), cereal::make_nvp("Max", m_max));
    }

private:
    T m_min;
    T m_max;
};

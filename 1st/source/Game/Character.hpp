#pragma once

#include "Utility.hpp"
#include "../Engine/STL.hpp"


// HitPoint
class HitPoint : public PlayDataEntity
{
public:
    typedef NE::U64 ValueType;
    typedef Range<ValueType> RangeType;

public:
    HitPoint();

    HitPoint(const Range<NE::U64>& range);

    inline ValueType Value() const { return m_value; }

    inline ValueType Max() const { return m_range.Max(); }

    inline ValueType Min() const { return m_range.Min(); }

    bool IsAlive() const;

    bool IsDead() const;

    void Damage(ValueType damage);

    void Recovery(ValueType recovery);

    void LevelUp(ValueType increase);
     
    // シリアライズ可能
    template<typename Archive>
    inline void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Value", m_value), cereal::make_nvp("Range", m_range));
    }

private:
    ValueType m_value;
    RangeType m_range;
};


// Attack
class Attack : public GameEntity
{
public:
    typedef NE::U64 ValueType;

public:
    Attack();

    explicit Attack(ValueType attack);

    inline ValueType Value() const { return m_value; }

private:
    ValueType m_value;
};


// Status
class Status : public PlayDataEntity
{
public:
    Status() = default;

    bool IsAlive() const;

    bool IsDead() const;

    void Hit(const Attack& attack);

    void LevelUp();

    template<typename Func>
    inline void Show(Func func)
    {
        func(m_hitPoint);
    }

    // シリアライズ可能
    template<typename Archive>
    inline void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("HitPoint", m_hitPoint));
    }

private:
    HitPoint m_hitPoint;
};


// Character
class Character : public GameEntity
{
public:
    Character() = default;
    ~Character() = default;

    bool IsAlive() const;

    bool IsDead() const;

    void HitTest(const Attack& attack);

    void LevelUp();

    template<typename Func>
    inline void ShowStatus(Func func)
    {
        m_status.Show(func);
    }

    // シリアライズ可能
    template<typename Archive>
    inline void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Status", m_status));
    }

private:
    Status m_status;
};

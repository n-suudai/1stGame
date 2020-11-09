#include "Character.hpp"


// HitPoint
HitPoint::HitPoint()
    : m_value()
    , m_range()
{
    m_value = m_range.Max(); // 最大値で初期化
}

HitPoint::HitPoint(const Range<NE::U64>& range)
    : m_value(range.Max()) // 最大値で初期化
    , m_range(range)
{}

bool HitPoint::IsAlive() const
{
    return m_value > m_range.Min();
}

bool HitPoint::IsDead() const
{
    return m_value <= m_range.Min();
}

void HitPoint::Damage(ValueType damage)
{
    damage = std::min(damage, m_value); // Unsigned なので負の値にできない
    m_value = m_range.Clamp(m_value - damage);
}

void HitPoint::Recovery(ValueType recovery)
{
    m_value = m_range.Clamp(m_value + recovery);
}

void HitPoint::LevelUp(ValueType increase)
{
    m_range = RangeType(m_range.Min(), m_range.Max() + increase); // 最大値増加
    m_value = m_range.Max(); // 回復
}


// Attack
Attack::Attack()
    : Attack(ValueType(0))
{}

Attack::Attack(ValueType attack)
    : m_value(attack)
{}


// Status
void Status::Hit(const Attack& attack)
{
    m_hitPoint.Damage(attack.Value());
}

bool Status::IsAlive() const
{
    return m_hitPoint.IsAlive();
}

bool Status::IsDead() const
{
    return m_hitPoint.IsDead();
}

void Status::LevelUp()
{
    m_hitPoint.LevelUp(10);
}


// Character
bool Character::IsAlive() const
{
    return m_status.IsAlive();
}

bool Character::IsDead() const
{
    return m_status.IsDead();
}

void Character::HitTest(const Attack& attack)
{
    m_status.Hit(attack); // 確実に当たる
}

void Character::LevelUp()
{
    m_status.LevelUp();
}

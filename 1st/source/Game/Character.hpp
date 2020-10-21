#pragma once


#include "Entities.hpp"
#include "../Engine/STL.hpp"


template<typename T>
class IStatusValue : public GameEntity
{
public:
    typedef T ValueType;

    virtual ~IStatusValue() = default;

    virtual void Initialize(const ValueType& value) = 0;

    virtual const ValueType& GetValue() const = 0;
};


template<typename T>
class StatusValue_Basic : public IStatusValue<T>
{
public:
    typedef typename IStatusValue<T>::ValueType ValueType;

    StatusValue_Basic() : m_value((ValueType)0) {}

    virtual ~StatusValue_Basic() = default;

    virtual void Initialize(const ValueType& value) override {
        m_value = value;
    }

    virtual const ValueType& GetValue() const override { return m_value; }

protected:
    ValueType m_value;
};


class Status : public GameEntity
{
public:
    Status() {}
    virtual ~Status() {}

private:
    NE::UniquePtr<IStatusValue<NE::U64>> m_hitPoint;
};


class Character : public GameEntity
{
public:
    Character() {}
    virtual ~Character() {}

private:
    Status m_status;
};


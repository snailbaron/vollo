#pragma once

#include <cmath>
#include <cassert>
#include <type_traits>

template <class T>
struct Vector {
    static_assert(std::is_arithmetic<T>(), "Vector must hold arithmetic type");

    T x;
    T y;

    constexpr Vector()
        : x{}, y{}
    { }

    constexpr Vector(T x, T y)
        : x(x), y(y)
    { }

    Vector& operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    T length() const
    {
        // TODO: do not calculate sqrt right away
        return sqrt(x * x + y * y);
    }

    Vector normalized() const
    {
        auto len = length();
        assert(len > 0);
        return {x / len, y / len};
    }

    Vector normalized(Vector default) const
    {
        auto len = length();
        if (len == 0) {
            return default;
        }
        return {x / len, y / len};
    }
};

template <class T>
Vector<T> operator-(Vector<T> vector)
{
    vector.x = -vector.x;
    vector.y = -vector.y;
    return vector;
}

template <class T>
Vector<T> operator+(Vector<T> left, const Vector<T>& right)
{
    left += right;
    return left;
}

template <class T>
Vector<T> operator-(Vector<T> left, const Vector<T>& right)
{
    left -= right;
    return left;
}

template <class T>
Vector<T> operator*(Vector<T> vector, T scalar)
{
    vector *= scalar;
    return vector;
}

template <class T>
Vector<T> operator*(T scalar, Vector<T> vector)
{
    vector *= scalar;
    return vector;
}

template <class Target, class Source>
Vector<Target> vectorCast(const Vector<Source>& source)
{
    return {static_cast<Target>(source.x), static_cast<Target>(source.y)};
}

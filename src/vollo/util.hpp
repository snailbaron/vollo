#pragma once

template <class T>
void clampLo(T& value, const T& minimum)
{
    value = std::max(value, minimum);
}

template <class T>
void clampHi(T& value, const T& maximum)
{
    value = std::min(value, maximum);
}

template <class T>
void clamp(T& value, const T& minimum, const T& maximum)
{
    value = std::min(std::max(value, minimum), maximum);
}

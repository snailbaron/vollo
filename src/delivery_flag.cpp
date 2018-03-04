#include "delivery_flag.hpp"

void DeliveryFlag::load()
{
    _value = true;
}

void DeliveryFlag::reset()
{
    _value = false;
}

DeliveryFlag::operator bool()
{
    auto result = _value;
    _value = false;
    return result;
}
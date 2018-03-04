#pragma once

#include "vector.hpp"

namespace ph {

struct AxisWall {
    enum class OpenSide {
        Positive,
        Negative,
    };

    enum class Alignment {
        Horizontal,
        Vertical,
    };

    double position;
    double bounce;
    OpenSide openSide;
    Alignment alignment;
};

struct Circle {
    Vector<double> position;
    Vector<double> velocity;
    double radius;
    double bounce;
};

template <class Left, class Right>
void collide(const Left& left, Right& right, double delta);

extern template void collide<AxisWall, Circle>(
    const AxisWall& left, Circle& right, double delta);
extern template void collide<Circle, Circle>(
    const Circle& left, Circle& right, double delta);

} // namespace ph
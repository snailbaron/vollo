#pragma once

#include <math.hpp>

namespace ph {

struct Plane {
    math::Line line;
};

struct Wall {

};

struct Sphere {
    math::Circle position;
    math::Vector<double> velocity;
    double bounce;
};

template <class Left, class Right>
void collide(const Left& left, Right& right, double delta);

} // namespace ph
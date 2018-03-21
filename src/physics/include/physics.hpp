#pragma once

#include <math.hpp>

namespace ph {

struct Wall {
    Wall(
        const math::Vector<double>& firstPoint,
        const math::Vector<double>& secondPoint);

    math::Line shape;
};

struct DynamicSphere {
    DynamicSphere(const math::Vector<double>& position = {}, double radius = 0);

    void update(double delta);

    math::Circle shape;
    math::Vector<double> velocity;
    double gravity = 0;
};

void resolveCollision(const Wall& wall, DynamicSphere& sphere);
void resolveCollision(const DynamicSphere& player, DynamicSphere& ball);

} // namespace ph
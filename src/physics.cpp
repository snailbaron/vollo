#include "physics.hpp"
#include "config.hpp"

#include <algorithm>
#include <optional>

namespace ph {

namespace {

struct Collision {
    Collision(const Vector<double>& normal, double penetration)
        : normal(normal)
        , penetration(penetration)
    { }

    Vector<double> normal;
    double penetration;
};

std::optional<Collision> findCollision(const Circle& left, const Circle& right)
{
    auto direction = right.position - left.position;
    auto penetration = left.radius + right.radius - direction.length();
    if (penetration <= 0) {
        return {};
    }

    auto normal = direction.normalized({0, 1});
    return {{normal, penetration}};
}

std::optional<Collision> findCollision(const AxisWall& wall, const Circle& circle)
{
    auto circlePosition = wall.alignment == AxisWall::Alignment::Horizontal ?
        circle.position.y : circle.position.x;
    auto distance = wall.position - circlePosition;
    if (wall.openSide == AxisWall::OpenSide::Negative) {
        distance = -distance;
    }
    auto penetration = distance + circle.radius;
    if (penetration <= 0) {
        return {};
    }

    auto normal = wall.alignment == AxisWall::Alignment::Vertical ?
        Vector<double>{1, 0} : Vector<double>{0, 1};
    if (wall.openSide == AxisWall::OpenSide::Negative) {
        normal = -normal;
    }

    return {{normal, penetration}};
}

} // namespace

template <class Left, class Right>
void collide(const Left& left, Right& right, double delta)
{
    auto collision = findCollision(left, right);
    if (!collision) {
        return;
    }

    auto bounce = std::min(left.bounce, right.bounce);
    auto friction = config::gameplay::CommonFriction;
    auto force =
        collision->penetration * collision->penetration * bounce;
    right.velocity += collision->normal * delta * force;
    right.velocity *= friction;
}

template void collide<AxisWall, Circle>(
    const AxisWall& left, Circle& right, double delta);
template void collide<Circle, Circle>(
    const Circle& left, Circle& right, double delta);

} // namespace ph
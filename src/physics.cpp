#include "physics.hpp"
#include "config.hpp"

#include <algorithm>
#include <optional>

using namespace math;

namespace ph {

namespace {

struct Collision {
    Collision(double time, const Vector<double>& normal)
        : time(time)
        , normal(normal)
    { }

    double time;
    Vector<double> normal;
};

std::optional<Collision> findCollision(
    const Plane& plane, const Sphere& circle, double delta)
{
    if (math::dot(circle.velocity, plane.line.normal) >= 0) {
        return {};
    }

    auto trajectory = Line::alongDirection(circle.position.center, circle.velocity);
    auto offsetLine = plane.line.move(circle.position.radius);
    auto intersection = lineCross(offsetLine, trajectory);
    if (!intersection) {
        return {};
    }

    auto travel = *intersection - circle.position.center;
    auto travelTime = travel.length() / circle.velocity.length();
    if (travelTime >= delta) {
        return {};
    }

    return {{travelTime, plane.line.normal}};
}

std::optional<Collision> staticBallCollision(const Sphere& staticBall, const Sphere& mover)
{
    auto traceResult = math::circleTraceCircle(
        mover.position, mover.velocity, staticBall.position);
    if (!traceResult) {
        return {};
    }

    auto normal = (*traceResult - staticBall.position.center).normalized();
    auto distance = (*traceResult - mover.position.center).length();
    auto time = distance / mover.velocity.length();
    return {{time, normal}};
}

std::optional<Collision> ballCollision(const Sphere& impactor, const Sphere& acceptor)
{
    auto fakeMover = acceptor;
    fakeMover.velocity -= impactor.velocity;
    return staticBallCollision(impactor, fakeMover);
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

} // namespace ph
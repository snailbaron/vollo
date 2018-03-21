#include <physics.hpp>
#include <config.hpp>

#include <algorithm>
#include <optional>

using namespace config::gameplay;

namespace ph {

Wall::Wall(
        const math::Vector<double>& firstPoint,
        const math::Vector<double>& secondPoint)
    : shape(math::Line::betweenPoints(firstPoint, secondPoint))
{ }

DynamicSphere::DynamicSphere(
        const math::Vector<double>& position, double radius)
    : shape(position, radius)
{ }

void DynamicSphere::update(double delta)
{
    velocity.y -= gravity;
    shape.center += velocity * delta;
}

void resolveCollision(const Wall& wall, DynamicSphere& sphere)
{
    if (distance(wall.shape, sphere.shape) > 0) {
        return;
    }

    auto wallMoveSign =
        dot(sphere.velocity, wall.shape.normal) < 0 ? 1.0 : -1.0;
    auto movedWall = wall.shape.move(wallMoveSign * sphere.shape.radius);
    auto ballAtCollision = math::rayTraceLine(
        sphere.shape.center, -sphere.velocity, movedWall);
    assert(ballAtCollision);
    auto diveDistance = math::distance(sphere.shape.center, *ballAtCollision);
    auto diveTime = diveDistance / sphere.velocity.length();

    sphere.shape.center = *ballAtCollision;
    auto velocityProjection = sphere.velocity.projection(wall.shape.normal);
    sphere.velocity -= 2.0 * velocityProjection;
    sphere.velocity *= WallHitFriction;
    sphere.update(diveTime);
}

void resolveCollision(const DynamicSphere& player, DynamicSphere& ball)
{
    if (distance(player.shape, ball.shape) > 0) {
        return;
    }

    math::Circle largePlayerBody(
        player.shape.center, player.shape.radius + ball.shape.radius);
    auto relativeBallVelocity = ball.velocity - player.velocity;
    auto relativeBallAtCollision = math::rayTraceCircle(
        ball.shape.center, -relativeBallVelocity, largePlayerBody);
    assert(relativeBallAtCollision);
    auto diveDistance =
        math::distance(ball.shape.center, *relativeBallAtCollision);
    auto diveTime = diveDistance / relativeBallVelocity.length();

    ball.shape.center -= diveTime * ball.velocity;
    auto hitPlayerCenter = player.shape.center - diveTime * player.velocity;
    auto hitDirection = (ball.shape.center - hitPlayerCenter).normalized();
    ball.velocity = ball.velocity.ort(hitDirection);
    ball.velocity += PlayerHitForce * hitDirection;
    ball.update(diveTime);
}

} // namespace ph
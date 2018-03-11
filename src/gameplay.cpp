#include "gameplay.hpp"
#include "util.hpp"
#include "config.hpp"

#include <algorithm>
#include <iostream>

using namespace config::gameplay;
using namespace math;

namespace {

void resolvePlayerBallCollision(const Player& player, Ball& ball)
{
    if (distance(player.body, ball.body) > 0) {
        return;
    }

    math::Circle largePlayerBody(
        player.body.center, player.body.radius + ball.body.radius);
    auto ballAtCollision = math::rayTraceCircle(
        ball.body.center, -ball.velocity, largePlayerBody);
    assert(ballAtCollision);
    auto diveDistance = math::distance(ball.body.center, *ballAtCollision);
    auto diveTime = diveDistance / ball.velocity.length();

    ball.body.center = *ballAtCollision;
    auto hitDirection =
        (ball.body.center - player.body.center).normalized();
    ball.velocity = ball.velocity.ort(hitDirection);
    ball.velocity += PlayerHitForce * hitDirection;
    ball.update(diveTime);
}

void resolveWallBallCollision(const math::Line& wall, Ball& ball)
{
    if (distance(wall, ball.body) > 0) {
        return;
    }

    auto wallMoveSign = dot(ball.velocity, wall.normal) < 0 ? 1.0 : -1.0;
    auto movedWall = wall.move(wallMoveSign * ball.body.radius);
    auto ballAtCollision = math::rayTraceLine(
        ball.body.center, -ball.velocity, movedWall);
    assert(ballAtCollision);
    auto diveDistance = math::distance(ball.body.center, *ballAtCollision);
    auto diveTime = diveDistance / ball.velocity.length();

    ball.body.center = *ballAtCollision;
    auto velocityProjection = ball.velocity.projection(wall.normal);
    ball.velocity -= 2.0 * velocityProjection;
    ball.velocity *= WallHitFriction;
    ball.update(diveTime);
}

} // namespace

Ball::Ball()
{
    body.radius = BallRadius;
}

void Ball::update(double delta)
{
    velocity.y -= BallGravity;
    body.center += velocity * delta;
}

Player::Player()
    : control(std::make_shared<Control>())
{
    body.radius = PlayerRadius;
}

void Player::update(double delta)
{
    // Apply controls
    velocity.x = control->movement * PlayerMoveSpeed;
    if (velocity.y == 0.0 && control->requestJump) {
        control->stopJump.reset();
        jumping = true;
        velocity.y = PlayerJumpSpeed;
    }

    if (velocity.y != 0.0 && control->stopJump) {
        jumping = false;
    }

    // Apply gravity
    velocity.y -= (jumping ? PlayerJumpGravity : PlayerGravity);

    // Move position
    body.center += velocity * delta;
    switch (side) {
        case Side::Left:
            clamp(body.center.x,
                PlayerRadius, 0.5 - SeparatorWidth / 2 - PlayerRadius);
            break;

        case Side::Right:
            clamp(body.center.x,
                0.5 + SeparatorWidth / 2 + PlayerRadius, 1.0 - PlayerRadius);
            break;
    }

    // Land, if required
    if (velocity.y != 0.0 && body.center.y <= PlayerRadius) {
        body.center.y = PlayerRadius;
        velocity.y = 0.0;
        control->requestJump.reset();
    }
}

Gameplay::Gameplay()
{
    _walls.push_back(Line::betweenPoints({0.0, 0.0}, {0.0, 1.0}));
    _walls.push_back(Line::betweenPoints({1.0, 0.0}, {1.0, 1.0}));
    _walls.push_back(Line::betweenPoints({0.0, 0.0}, {1.0, 0.0}));

    _leftPlayer.body.center = {PlayerRadius, PlayerRadius};
    _leftPlayer.side = Player::Side::Left;
    _rightPlayer.body.center = {1 - PlayerRadius, PlayerRadius};
    _rightPlayer.side = Player::Side::Right;
    _ball.body.center = {0.5, 0.5};
}

void Gameplay::update(double delta)
{
    _leftPlayer.update(delta);
    _rightPlayer.update(delta);
    _ball.update(delta);

    for (const auto& wall : _walls) {
        resolveWallBallCollision(wall, _ball);
    }
    resolvePlayerBallCollision(_leftPlayer, _ball);
    resolvePlayerBallCollision(_rightPlayer, _ball);
}

std::weak_ptr<Control> Gameplay::playerOneControl() const
{
    return _leftPlayer.control;
}

std::weak_ptr<Control> Gameplay::playerTwoControl() const
{
    return _rightPlayer.control;
}

const Vector<double>& Gameplay::playerOnePosition() const
{
    return _leftPlayer.body.center;
}

const Vector<double>& Gameplay::playerTwoPosition() const
{
    return _rightPlayer.body.center;
}

const Vector<double>& Gameplay::ballPosition() const
{
    return _ball.body.center;
}


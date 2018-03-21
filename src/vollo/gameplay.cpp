#include "gameplay.hpp"
#include "util.hpp"
#include "config.hpp"

#include <algorithm>
#include <iostream>

using namespace config::gameplay;
using namespace math;

Player::Player(Side side)
    : control(std::make_shared<Control>())
    , side(side)
{
    body.shape.radius = PlayerRadius;
    body.shape.center.y = PlayerRadius;
    body.shape.center.x = side == Side::Left ? 0.25 : 0.75;
}

void Player::update(double delta)
{
    // Apply controls
    body.velocity.x = control->movement * PlayerMoveSpeed;
    if (body.velocity.y == 0.0 && control->requestJump) {
        control->stopJump.reset();
        body.gravity = PlayerJumpGravity;
        body.velocity.y = PlayerJumpSpeed;
    }

    if (body.velocity.y != 0.0 && control->stopJump) {
        body.gravity = PlayerGravity;
    }

    // Apply gravity
    body.velocity.y -= (jumping ? PlayerJumpGravity : PlayerGravity);

    // Move position
    body.shape.center += body.velocity * delta;
    switch (side) {
        case Side::Left:
            clamp(body.shape.center.x,
                PlayerRadius, 0.5 - SeparatorWidth / 2 - PlayerRadius);
            break;

        case Side::Right:
            clamp(body.shape.center.x,
                0.5 + SeparatorWidth / 2 + PlayerRadius, 1.0 - PlayerRadius);
            break;
    }

    // Land, if required
    if (body.velocity.y != 0.0 && body.shape.center.y <= PlayerRadius) {
        body.shape.center.y = PlayerRadius;
        body.velocity.y = 0.0;
        control->requestJump.reset();
    }
}

Gameplay::Gameplay()
{
    _walls.push_back({{0.0, 0.0}, {0.0, 1.0}});
    _walls.push_back({{1.0, 0.0}, {1.0, 1.0}});
    _walls.push_back({{0.0, 0.0}, {1.0, 0.0}});

    _players.emplace_back(Player::Side::Left);
    _players.emplace_back(Player::Side::Right);

    _ball.shape.center = {0.35, 0.5};
}

void Gameplay::update(double delta)
{
    for (auto& player : _players) {
        player.update(delta);
    }
    _ball.update(delta);

    for (const auto& wall : _walls) {
        ph::resolveCollision(wall, _ball);
    }

    for (const auto& player : _players) {
        ph::resolveCollision(player.body, _ball);
    }
}

std::weak_ptr<Control> Gameplay::playerOneControl() const
{
    return _players.at(0).control;
}

std::weak_ptr<Control> Gameplay::playerTwoControl() const
{
    return _players.at(1).control;
}

const Vector<double>& Gameplay::playerOnePosition() const
{
    return _players.at(0).body.shape.center;
}

const Vector<double>& Gameplay::playerTwoPosition() const
{
    return _players.at(1).body.shape.center;
}

const Vector<double>& Gameplay::ballPosition() const
{
    return _ball.shape.center;
}


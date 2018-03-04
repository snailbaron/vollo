#include "gameplay.hpp"
#include "util.hpp"
#include "events.hpp"
#include "config.hpp"

#include <algorithm>
#include <iostream>

using namespace config::gameplay;

Ball::Ball()
{
    body.radius = BallRadius;
    body.bounce = BallBounce;
}

void Ball::update(double delta)
{
    body.velocity.y -= BallGravity;
    body.position += body.velocity * delta;
}

Player::Player()
    : control(std::make_shared<Control>())
{
    body.radius = PlayerRadius;
    body.bounce = PlayerBounce;
}

void Player::update(double delta)
{
    // Apply controls
    body.velocity.x = control->movement * PlayerMoveSpeed;
    if (body.velocity.y == 0.0 && control->requestJump) {
        control->stopJump.reset();
        jumping = true;
        body.velocity.y = PlayerJumpSpeed;
    }

    if (body.velocity.y != 0.0 && control->stopJump) {
        jumping = false;
    }

    // Apply gravity
    body.velocity.y -= (jumping ? PlayerJumpGravity : PlayerGravity);

    // Move position
    body.position += body.velocity * delta;
    switch (side) {
        case Side::Left:
            clamp(body.position.x,
                PlayerRadius, 0.5 - SeparatorWidth / 2 - PlayerRadius);
            break;

        case Side::Right:
            clamp(body.position.x,
                0.5 + SeparatorWidth / 2 + PlayerRadius, 1.0 - PlayerRadius);
            break;
    }

    // Land, if required
    if (body.velocity.y != 0.0 && body.position.y <= PlayerRadius) {
        body.position.y = PlayerRadius;
        body.velocity.y = 0.0;
        control->requestJump.reset();
    }
}

Gameplay::Gameplay()
{
    {
        ph::AxisWall wall;
        wall.bounce = WallBounce;
        wall.position = 0.0;
        wall.alignment = ph::AxisWall::Alignment::Horizontal;
        wall.openSide = ph::AxisWall::OpenSide::Positive;
        _walls.push_back(wall);
    }

    {
        ph::AxisWall wall;
        wall.bounce = WallBounce;
        wall.position = 0.0;
        wall.alignment = ph::AxisWall::Alignment::Vertical;
        wall.openSide = ph::AxisWall::OpenSide::Positive;
        _walls.push_back(wall);
    }

    {
        ph::AxisWall wall;
        wall.bounce = WallBounce;
        wall.position = 1.0;
        wall.alignment = ph::AxisWall::Alignment::Vertical;
        wall.openSide = ph::AxisWall::OpenSide::Negative;
        _walls.push_back(wall);
    }

    _leftPlayer.body.position = {PlayerRadius, PlayerRadius};
    _leftPlayer.side = Player::Side::Left;
    _rightPlayer.body.position = {1 - PlayerRadius, PlayerRadius};
    _rightPlayer.side = Player::Side::Right;
    _ball.body.position = {0.5, 0.5};
}

void Gameplay::update(double delta)
{
    _leftPlayer.update(delta);
    event::bus.push(event::LeftPlayerMove{_leftPlayer.body.position});

    _rightPlayer.update(delta);
    event::bus.push(event::RightPlayerMove{_rightPlayer.body.position});

    _ball.update(delta);
    event::bus.push(event::BallMove{_ball.body.position});

    ph::collide(_leftPlayer.body, _ball.body, delta);
    ph::collide(_rightPlayer.body, _ball.body, delta);

    for (const auto& wall : _walls) {
        ph::collide(wall, _ball.body, delta);
    }
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
    return _leftPlayer.body.position;
}

const Vector<double>& Gameplay::playerTwoPosition() const
{
    return _rightPlayer.body.position;
}

const Vector<double>& Gameplay::ballPosition() const
{
    return _ball.body.position;
}

#include "gameplay.hpp"
#include "util.hpp"
#include "events.hpp"
#include "config.hpp"

#include <algorithm>

using namespace config::gameplay;

namespace {

void processCollision(const Player& player, Ball& ball)
{
    auto playerToBall = ball.position - player.position;
    auto distance = playerToBall.length();
    if (distance < PlayerRadius + BallRadius) {
        if (distance == 0.0) {
            playerToBall = {0, 1};
        }
        ball.velocity = playerToBall.normalized() * BallImpulse;
    }
}

} // namespace

void Ball::update(double delta)
{
    velocity.y -= BallGravity;
    position += velocity * delta;
}

Player::Player()
    : control(std::make_shared<Control>())
{ }

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
    position += velocity * delta;
    switch (side) {
        case Side::Left:
            clamp(position.x,
                PlayerRadius, 0.5 - SeparatorWidth / 2 - PlayerRadius);
            break;

        case Side::Right:
            clamp(position.x,
                0.5 + SeparatorWidth / 2 + PlayerRadius, 1.0 - PlayerRadius);
            break;
    }

    // Land, if required
    if (velocity.y != 0.0 && position.y <= PlayerRadius) {
        position.y = PlayerRadius;
        velocity.y = 0.0;
        control->requestJump.reset();
    }
}

Gameplay::Gameplay()
{
    _leftPlayer.position = {PlayerRadius, PlayerRadius};
    _leftPlayer.side = Player::Side::Left;
    _rightPlayer.position = {1 - PlayerRadius, PlayerRadius};
    _rightPlayer.side = Player::Side::Right;
    _ball.position = {0.5, 0.5};
}

void Gameplay::update(double delta)
{
    _leftPlayer.update(delta);
    event::bus.push(event::LeftPlayerMove{_leftPlayer.position});

    _rightPlayer.update(delta);
    event::bus.push(event::RightPlayerMove{_rightPlayer.position});

    _ball.update(delta);
    event::bus.push(event::BallMove{_ball.position});

    processCollision(_leftPlayer, _ball);
    processCollision(_rightPlayer, _ball);

    if (_ball.position.x < BallRadius) {
        _ball.position.x = BallRadius;
        _ball.velocity.x *= -ImpactFriction;
    }
    if (_ball.position.x > 1 - BallRadius) {
        _ball.position.x = 1 - BallRadius;
        _ball.velocity.x *= -ImpactFriction;
    }
    if (_ball.position.y < BallRadius) {
        _ball.position.y = BallRadius;
        _ball.velocity.y *= -ImpactFriction;
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
    return _leftPlayer.position;
}

const Vector<double>& Gameplay::playerTwoPosition() const
{
    return _rightPlayer.position;
}

const Vector<double>& Gameplay::ballPosition() const
{
    return _ball.position;
}

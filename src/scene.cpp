#include "scene.hpp"
#include "config.hpp"

#include <cassert>

using namespace math;

namespace client {

using namespace config::client;

Player::Player(const sf::Color& color)
{
    auto radius = static_cast<float>(PlayerRadius);
    _shape.setRadius(radius);
    _shape.setOrigin(radius, radius);
    _shape.setFillColor(color);
}

void Player::setPosition(const Vector<float>& position)
{
    _shape.setPosition(position.x, position.y);
}

void Player::render(sf::RenderTarget& target) const
{
    target.draw(_shape);
}

Ball::Ball()
{
    auto radius = static_cast<float>(BallRadius);
    _shape.setRadius(radius);
    _shape.setOrigin(radius, radius);
}

void Ball::setPosition(const Vector<float>& position)
{
    _shape.setPosition(position.x, position.y);
}

void Ball::render(sf::RenderTarget& target) const
{
    target.draw(_shape);
}

Separator::Separator()
{
    auto width = static_cast<float>(config::gameplay::SeparatorWidth);
    auto height = static_cast<float>(config::gameplay::SeparatorHeight);
    auto tipRadius = width / 2;

    _base.setSize({width, height - tipRadius});
    _base.setOrigin(tipRadius, 0.f);
    _base.setPosition(0.5f, 0.f);
    _base.setFillColor(sf::Color::Yellow);

    _tip.setRadius(tipRadius);
    _tip.setOrigin(tipRadius, tipRadius);
    _tip.setPosition(0.5f, height - tipRadius);
    _tip.setFillColor(sf::Color::Yellow);
}

void Separator::render(sf::RenderTarget& target) const
{
    target.draw(_base);
    target.draw(_tip);
}

Shadow::Shadow()
{
    auto width = 2 * static_cast<float>(config::gameplay::BallRadius);
    auto height = config::client::ShadowHeight;

    _shape.setSize({width, height});
    _shape.setOrigin(width / 2, height);
    _shape.setPosition(0, -height / 2);
    _shape.setFillColor(sf::Color::Blue);
}

void Shadow::setPosition(float position)
{
    _shape.setPosition(position, _shape.getPosition().y);
}

void Shadow::render(sf::RenderTarget& target) const
{
    target.draw(_shape);
}

Scene::Scene(
        const std::shared_ptr<sf::RenderTarget>& renderTarget,
        const std::shared_ptr<Gameplay>& gameplay)
    : _renderTarget(renderTarget)
    , _gameplay(gameplay)
    , _leftPlayer(sf::Color::Red)
    , _rightPlayer(sf::Color::Green)
{
    assert(_renderTarget);
    assert(_gameplay);

    auto screenSize = static_cast<sf::Vector2f>(renderTarget->getSize());
    auto fieldHeight = screenSize.y / screenSize.x - FloorBorder;

    _view.reset({
        0, fieldHeight,
        1, -fieldHeight - FloorBorder});

    _renderTarget->setView(_view);
}

void Scene::update(double /*delta*/)
{
    _leftPlayer.setPosition(vectorCast<float>(_gameplay->playerOnePosition()));
    _rightPlayer.setPosition(vectorCast<float>(_gameplay->playerTwoPosition()));
    _ball.setPosition(vectorCast<float>(_gameplay->ballPosition()));
    _shadow.setPosition(static_cast<float>(_gameplay->ballPosition().x));
}

void Scene::render() const
{
    _shadow.render(*_renderTarget);
    _separator.render(*_renderTarget);
    _leftPlayer.render(*_renderTarget);
    _rightPlayer.render(*_renderTarget);
    _ball.render(*_renderTarget);
}

} // namespace client

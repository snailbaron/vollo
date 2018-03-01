#include "scene.hpp"
#include "config.hpp"

namespace client {

using namespace config::client;

Player::Player(
        const std::shared_ptr<sf::RenderTarget>& renderTarget,
        const sf::Color& color)
    : _renderTarget(renderTarget)
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

void Player::render(const sf::Transform& transform) const
{
    _renderTarget->draw(_shape, transform);
}

Ball::Ball(const std::shared_ptr<sf::RenderTarget>& renderTarget)
    : _renderTarget(renderTarget)
{
    auto radius = static_cast<float>(BallRadius);
    _shape.setRadius(radius);
    _shape.setOrigin(radius, radius);
}

void Ball::setPosition(const Vector<float>& position)
{
    _shape.setPosition(position.x, position.y);
}

void Ball::render(const sf::Transform& transform) const
{
    _renderTarget->draw(_shape, transform);
}

Scene::Scene(const std::shared_ptr<sf::RenderTarget>& renderTarget)
    : _renderTarget(renderTarget)
    , _leftPlayer(renderTarget, sf::Color::Red)
    , _rightPlayer(renderTarget, sf::Color::Green)
    , _ball(renderTarget)
{
    _screenTransform = sf::Transform();

    subscribe<event::LeftPlayerMove>(event::bus, [this] (const auto& event) {
        _leftPlayer.setPosition(vectorCast<float>(event.position));
    });
    subscribe<event::RightPlayerMove>(event::bus, [this] (const auto& event) {
        _rightPlayer.setPosition(vectorCast<float>(event.position));
    });
    subscribe<event::BallMove>(event::bus, [this] (const auto& event) {
        _ball.setPosition(vectorCast<float>(event.position));
    });
}

void Scene::render() const
{
    auto screenSize = static_cast<sf::Vector2f>(_renderTarget->getSize());
    auto fieldHeight = screenSize.y / screenSize.x - FloorBorder;

    sf::View view({
        0, fieldHeight,
        1, -fieldHeight - FloorBorder});
    _renderTarget->setView(view);

    _leftPlayer.render(_screenTransform);
    _rightPlayer.render(_screenTransform);
    _ball.render(_screenTransform);
}

} // namespace client

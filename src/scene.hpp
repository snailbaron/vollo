#pragma once

#include "events.hpp"
#include "vector.hpp"

#include <SFML/Graphics.hpp>

#include <memory>

namespace client {

class Player {
public:
    Player(
        const std::shared_ptr<sf::RenderTarget>& renderTarget,
        const sf::Color& color);

    void setPosition(const Vector<float>& position);
    void render(const sf::Transform& transform) const;

private:
    std::shared_ptr<sf::RenderTarget> _renderTarget;
    sf::CircleShape _shape;
};

class Ball {
public:
    Ball(const std::shared_ptr<sf::RenderTarget>& renderTarget);

    void setPosition(const Vector<float>& position);
    void render(const sf::Transform& transform) const;

private:
    std::shared_ptr<sf::RenderTarget> _renderTarget;
    sf::CircleShape _shape;
};

class Scene : public evening::Subscriber {
public:
    Scene(const std::shared_ptr<sf::RenderTarget>& renderTarget);

    void render() const;

private:
    Player _leftPlayer;
    Player _rightPlayer;
    Ball _ball;

    std::shared_ptr<sf::RenderTarget> _renderTarget;
    sf::Transform _screenTransform;
};

} // namespace client
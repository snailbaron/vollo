#pragma once

#include "events.hpp"
#include "vector.hpp"
#include "gameplay.hpp"

#include <SFML/Graphics.hpp>

#include <memory>

namespace client {

class Player {
public:
    Player(const sf::Color& color);

    void setPosition(const Vector<float>& position);
    void render(sf::RenderTarget& target) const;

private:
    sf::CircleShape _shape;
};

class Ball {
public:
    Ball();

    void setPosition(const Vector<float>& position);
    void render(sf::RenderTarget& target) const;

private:
    sf::CircleShape _shape;
};

class Separator {
public:
    Separator();

    void render(sf::RenderTarget& target) const;

private:
    sf::RectangleShape _base;
    sf::CircleShape _tip;
};

class Shadow {
public:
    Shadow();

    void setPosition(float position);
    void render(sf::RenderTarget& target) const;

private:
    sf::RectangleShape _shape;
};

class Scene : public evening::Subscriber {
public:
    Scene(
        const std::shared_ptr<sf::RenderTarget>& renderTarget,
        const std::shared_ptr<Gameplay>& gameplay);

    void update(double delta);
    void render() const;

private:
    Player _leftPlayer;
    Player _rightPlayer;
    Ball _ball;
    Separator _separator;
    Shadow _shadow;

    std::shared_ptr<sf::RenderTarget> _renderTarget;
    std::shared_ptr<Gameplay> _gameplay;
    sf::View _view;
    sf::Transform _screenTransform;
};

} // namespace client
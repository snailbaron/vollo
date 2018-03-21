#pragma once

#include "controller.hpp"
#include <physics.hpp>

#include <memory>

struct Player {
    enum class Side {
        Left,
        Right,
    };

    Player(Side side);

    void update(double delta);

    ph::DynamicSphere body;
    std::shared_ptr<Control> control;
    Side side;
    bool jumping = false;
};

class Gameplay {
public:
    Gameplay();

    void update(double delta);

    std::weak_ptr<Control> playerOneControl() const;
    std::weak_ptr<Control> playerTwoControl() const;

    const math::Vector<double>& playerOnePosition() const;
    const math::Vector<double>& playerTwoPosition() const;
    const math::Vector<double>& ballPosition() const;

private:
    std::vector<Player> _players;
    ph::DynamicSphere _ball;
    std::vector<ph::Wall> _walls;
};
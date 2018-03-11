#pragma once

#include "controller.hpp"
#include <math.hpp>

#include <memory>

struct Player {
    enum class Side {
        Left,
        Right,
    };

    Player();

    void update(double delta);

    math::Circle body;
    math::Vector<double> velocity;
    std::shared_ptr<Control> control;
    Side side;
    bool jumping = false;
};

struct Ball {
    Ball();

    void update(double delta);

    math::Circle body;
    math::Vector<double> velocity;
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
    Player _leftPlayer;
    Player _rightPlayer;
    Ball _ball;
    std::vector<math::Line> _walls;
};
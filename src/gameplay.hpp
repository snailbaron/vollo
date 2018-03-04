#pragma once

#include "vector.hpp"
#include "controller.hpp"
#include "physics.hpp"

#include <memory>

struct SpatialState {
    Vector<double> position;
    Vector<double> velocity;

    void update(double delta);
};

struct Player {
    enum class Side {
        Left,
        Right,
    };

    Player();

    void update(double delta);

    ph::Circle body;
    std::shared_ptr<Control> control;
    Side side;
    bool jumping = false;
};

struct Ball {
    Ball();

    void update(double delta);

    ph::Circle body;
};

class Gameplay {
public:
    Gameplay();

    void update(double delta);

    std::weak_ptr<Control> playerOneControl() const;
    std::weak_ptr<Control> playerTwoControl() const;

    const Vector<double>& playerOnePosition() const;
    const Vector<double>& playerTwoPosition() const;
    const Vector<double>& ballPosition() const;

private:
    Player _leftPlayer;
    Player _rightPlayer;
    Ball _ball;
    std::vector<ph::AxisWall> _walls;
};
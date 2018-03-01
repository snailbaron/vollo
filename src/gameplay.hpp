#pragma once

#include "vector.hpp"
#include "controller.hpp"

#include <memory>

struct SpatialState {
    Vector<double> position;
    Vector<double> velocity;

    void update(double delta);
};

struct Player {
    Player();

    void update(double delta);

    Vector<double> position;
    Vector<double> velocity;
    std::shared_ptr<Control> control;
};

struct Ball {
    Vector<double> position;
    Vector<double> velocity;

    void update(double delta);
};

class Gameplay {
public:
    Gameplay();

    void update(double delta);

    std::weak_ptr<Control> playerOneControl() const;
    std::weak_ptr<Control> playerTwoControl() const;

private:
    Player _leftPlayer;
    Player _rightPlayer;
    Ball _ball;
};
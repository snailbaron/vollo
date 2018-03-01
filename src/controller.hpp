#pragma once

#include "config.hpp"

#include <SFML/Window.hpp>

#include <memory>

struct Control {
    double movement = 0.0;
    bool jump = false;
};

class Controller {
public:
    Controller(
        std::weak_ptr<Control> control,
        const config::client::ControlScheme& controlScheme);

    void processEvent(const sf::Event& event);

private:
    std::weak_ptr<Control> _control;
    config::client::ControlScheme _scheme;
};
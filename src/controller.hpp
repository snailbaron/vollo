#pragma once

#include "config.hpp"
#include "delivery_flag.hpp"

#include <SFML/Window.hpp>

#include <memory>

struct Control {
    double movement = 0.0;
    DeliveryFlag requestJump;
    DeliveryFlag stopJump;
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
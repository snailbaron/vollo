#include "controller.hpp"

#include <utility>

Controller::Controller(
        std::weak_ptr<Control> control,
        const config::client::ControlScheme& controlScheme)
    : _control(std::move(control))
    , _scheme(controlScheme)
{ }

void Controller::processEvent(const sf::Event& event)
{
    auto control = _control.lock();
    if (!control) {
        return;
    }

    if (event.type != sf::Event::KeyPressed &&
            event.type != sf::Event::KeyReleased) {
        return;
    }

    auto key = event.key.code;
    if (key == _scheme.moveLeftKey || key == _scheme.moveRightKey) {
        control->movement = 0.0;
        if (sf::Keyboard::isKeyPressed(_scheme.moveLeftKey)) {
            control->movement -= 1.0;
        }
        if (sf::Keyboard::isKeyPressed(_scheme.moveRightKey)) {
            control->movement += 1.0;
        }
    } else if (event.type == sf::Event::KeyPressed && key == _scheme.jumpKey) {
        control->jump = true;
    }
}

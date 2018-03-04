#include "timer.hpp"
#include "gameplay.hpp"
#include "events.hpp"
#include "scene.hpp"
#include "controller.hpp"

#include <SFML/Graphics.hpp>

#include <functional>

constexpr auto GAME_FPS = 60;

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    auto window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1280, 720), "Vollo", sf::Style::Default, settings);
    window->setKeyRepeatEnabled(false);

    auto core = std::make_shared<Gameplay>();
    client::Scene scene(window, core);
    Controller playerOneController(
        core->playerOneControl(), config::client::playerOneControls);
    Controller playerTwoController(
        core->playerTwoControl(), config::client::playerTwoControls);

    FrameTimer timer(GAME_FPS);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                continue;
            }

            playerOneController.processEvent(event);
            playerTwoController.processEvent(event);
        }

        int frames = timer.framesPassed();
        if (frames == 0) {
            continue;
        }

        for (int i = 0; i < frames; i++) {
            core->update(timer.delta());
        }

        event::bus.deliver();

        scene.update(timer.delta() * frames);

        window->clear(sf::Color::Black);
        scene.render();
        window->display();
    }
}
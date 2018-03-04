#pragma once

#include <SFML/Graphics.hpp>

namespace config {
    namespace gameplay {
        constexpr auto PlayerRadius = 0.04;
        constexpr auto BallRadius = 0.03;
        constexpr auto ImpactFriction = 0.9;
        constexpr auto BallImpulse = 0.7;

        constexpr auto BallGravity = 0.01;
        constexpr auto PlayerGravity = 0.04;
        constexpr auto PlayerJumpGravity = 0.013;

        constexpr auto PlayerMoveSpeed = 0.4;
        constexpr auto PlayerJumpSpeed = 0.6;

        constexpr auto SeparatorWidth = 0.02;
        constexpr auto SeparatorHeight = 0.25;
    }

    namespace client {
        struct ControlScheme {
            sf::Keyboard::Key moveLeftKey;
            sf::Keyboard::Key moveRightKey;
            sf::Keyboard::Key jumpKey;
        };

        constexpr auto PlayerRadius =
            static_cast<float>(gameplay::PlayerRadius);
        constexpr auto BallRadius =
            static_cast<float>(gameplay::BallRadius);

        constexpr auto PlayerPointCount = 32;
        const sf::Color BallColor = sf::Color::White;
        constexpr auto FloorBorder = 0.05f;
        constexpr auto ShadowHeight = 0.02f;

        inline const ControlScheme playerOneControls {
            sf::Keyboard::A,
            sf::Keyboard::D,
            sf::Keyboard::W
        };
        inline const ControlScheme playerTwoControls {
            sf::Keyboard::Left,
            sf::Keyboard::Right,
            sf::Keyboard::Up
        };
    }
}

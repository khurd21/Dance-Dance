#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

#include <iostream>
#include <memory>
#include <unordered_map>

#include <DanceDance/GameState.hpp>
#include <DanceDance/IScene.hpp>
#include <DanceDance/MainMenu.hpp>

int main() {
    constexpr auto targetFPS = 60.f;
    constexpr auto frameTime = 1.f / targetFPS;

    const auto videoMode{sf::VideoMode::getFullscreenModes().at(0)};
    sf::RenderWindow window(videoMode, "Dance Dance", sf::State::Fullscreen);
    sf::Font font("resources/fonts/perfect-delight-1992.otf");

    sf::Clock clock;
    auto currentState = dd::GameState::Home;
    std::unordered_map<dd::GameState, std::unique_ptr<dd::IScene>> states;
    states.emplace(dd::GameState::Home, std::make_unique<dd::MainMenu>(font, videoMode));

    while (window.isOpen()) {
        auto elapsed = clock.restart().asSeconds();
        if (elapsed < frameTime) {
            sf::sleep(sf::seconds(frameTime - elapsed));
            elapsed = frameTime;
        }

        auto& scene{*states.at(currentState)};
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
            scene.handleEvent(event.value());
        }

        scene.update(elapsed);
        window.clear();
        window.draw(scene);
        window.display();
    }

    return 0;
}
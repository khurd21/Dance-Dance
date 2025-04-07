#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include <DanceDance/EventSystem.hpp>
#include <DanceDance/Game.hpp>
#include <DanceDance/GameEvents.hpp>
#include <DanceDance/MainMenu.hpp>
#include <DanceDance/Runner.hpp>

#include <memory>

namespace dd {

Runner::Runner() : m_currentState(GameState::Home), m_font("assets/fonts/perfect-delight-1992.otf") {
    const auto videoMode = sf::VideoMode::getFullscreenModes().at(0);
    m_renderWindow = sf::RenderWindow(videoMode, "Dance Dance", sf::State::Fullscreen);
    m_states.emplace(GameState::Home, std::make_unique<MainMenu>(m_font, videoMode, m_eventSystem));
    m_states.emplace(GameState::Play, std::make_unique<Game>(m_eventSystem));
    m_eventSystem.subscribe<ExitGameEvent>([this](const auto&) { m_renderWindow.close(); });
    m_eventSystem.subscribe<GameStateChangeEvent>([this](const auto& event) { m_currentState = event.to; });
}

void Runner::run() {
    constexpr auto targetFPS = 60.f;
    constexpr auto frameTime = 1.f / targetFPS;

    while (m_renderWindow.isOpen()) {
        auto elapsed = m_clock.restart().asSeconds();
        if (elapsed < frameTime) {
            sf::sleep(sf::seconds(frameTime - elapsed));
            elapsed = frameTime;
        }

        auto& scene = *m_states.at(m_currentState);
        while (const auto event = m_renderWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_renderWindow.close();
            }
            if (const auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::Key::Escape == keyPressed->code) {
                    m_renderWindow.close();
                }
            }
            scene.handleEvent(event.value());
        }

        scene.update(elapsed);
        m_renderWindow.clear();
        m_renderWindow.draw(scene);
        m_renderWindow.display();
    }
}

} // namespace dd
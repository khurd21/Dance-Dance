#include <DanceDance/EventSystem.hpp>
#include <DanceDance/Game.hpp>
#include <DanceDance/GameEvents.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace dd {

Game::Game(const sf::VideoMode& videoMode, EventSystem& eventSystem)
    : m_defaultTexture("assets/images/back-button-white-32x32.png"), m_backButton(m_defaultTexture), m_eventSystem(eventSystem),
      m_videoMode(videoMode) {
    m_defaultTexture.setSmooth(true);
    m_backButton.setPosition({20.f, 20.f});
    m_backButton.setScale({1.2f, 1.2f});

    constexpr auto arrowCount = 4;
    const auto y = m_videoMode.size.y / 6.f;
    const auto centerX = m_videoMode.size.x / 2.f;
    const auto totalWidth = m_videoMode.size.x * 0.3f;
    const auto spacing = totalWidth / (arrowCount - 1);

    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Left, {centerX - totalWidth / 2.f + 0 * spacing, y}));
    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Up, {centerX - totalWidth / 2.f + 1 * spacing, y}));
    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Down, {centerX - totalWidth / 2.f + 2 * spacing, y}));
    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Right, {centerX - totalWidth / 2.f + 3 * spacing, y}));
}

void Game::handleEvent(const sf::Event& event) {
    if (const auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_isHovering = m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseMoved->position));
    }
    if (const auto mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseReleased->position))) {
            m_eventSystem.publish(GameStateChangeEvent{.from = GameState::Play, .to = GameState::Home});
        }
    }
}

void Game::update(float dt) { m_backButton.setColor(m_isHovering ? sf::Color(255, 200, 200) : sf::Color::White); }

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    std::ranges::for_each(m_stationaryArrows, [&](const auto& arrow) { target.draw(arrow, states); });
    target.draw(m_backButton, states);
}

} // namespace dd
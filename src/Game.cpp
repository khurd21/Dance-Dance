#include <DanceDance/EventSystem.hpp>
#include <DanceDance/Game.hpp>
#include <DanceDance/GameEvents.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <utility>

namespace dd {

Game::Game(EventSystem& eventSystem)
    : m_defaultTexture("assets/images/back-button-white-32x32.png"), m_backButton(m_defaultTexture), m_eventSystem(eventSystem) {
    m_defaultTexture.setSmooth(true);
    m_backButton.setPosition({20.f, 20.f});
    m_backButton.setScale({1.2f, 1.2f});
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

void Game::update(float /*dt*/) { m_backButton.setColor(m_isHovering ? sf::Color(255, 200, 200) : sf::Color::White); }

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(m_backButton, states); }

} // namespace dd
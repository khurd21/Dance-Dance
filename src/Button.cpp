#include <DanceDance/Button.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

#include <DanceDance/EventSystem.hpp>
#include <DanceDance/GameEvents.hpp>

namespace dd {

Button::Button(const std::string& label, const sf::Font& font, const sf::Vector2f& position, EventSystem& eventSystem)
    : m_text(sf::Text(font, label, 80)), m_eventSystem(eventSystem) {
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position);
    m_text.setOrigin({m_text.getLocalBounds().size.x / 2.f, m_text.getLocalBounds().size.y / 2.f});
}

void Button::handleEvent(const sf::Event& event) {
    if (const auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        const auto& position = mouseMoved->position;
        m_isHovering = m_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(position));
    }
    if (const auto mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (sf::Mouse::Button::Left == mouseReleased->button) {
            const auto& position = mouseReleased->position;
            m_isSelected = m_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(position));
            if (m_isSelected) {
                m_eventSystem.publish(ButtonClickedEvent{.buttonName = m_text.getString()});
            }
        }
    }
}

void Button::update(float dt) { m_text.setFillColor(m_isHovering ? sf::Color(255, 200, 200) : sf::Color::White); }

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(m_text); }

} // namespace dd

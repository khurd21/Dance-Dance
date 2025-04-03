#include <DanceDance/Button.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

namespace dd {

Button::Button(const std::string& label, const sf::Font& font, const sf::Vector2f& position) : m_text(sf::Text(font, label, 24)) {
    m_rectangleShape.setSize({200.f, 50.f});
    m_rectangleShape.setFillColor(sf::Color::White);
    m_rectangleShape.setPosition(position);

    m_text.setFillColor(sf::Color::White);
    m_text.setPosition({position.x + (m_rectangleShape.getSize().x - m_text.getLocalBounds().size.x) / 2,
                        position.y + (m_rectangleShape.getSize().y - m_text.getLocalBounds().size.y) / 2});
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
        }
    }
}

void Button::update(float dt) {}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_rectangleShape);
    target.draw(m_text);
}

} // namespace dd

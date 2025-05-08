#include <DanceDance/Button.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
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

void Button::handleEvent(const sf::Event& event, sf::View* view) {
    const auto checkCollision = [this](const sf::Vector2f& mousePos, const sf::View* view) {
        if (!view) {
            return m_text.getGlobalBounds().contains(mousePos);
        }
        const auto viewMousePos = sf::Vector2f{(mousePos.x - view->getViewport().position.x * view->getSize().x) / view->getViewport().size.x,
                                               (mousePos.y - view->getViewport().position.y * view->getSize().y) / view->getViewport().size.y};
        const auto transformedBounds = m_text.getTransform().transformRect(m_text.getLocalBounds());
        return transformedBounds.contains(viewMousePos);
    };

    if (const auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        const auto position = static_cast<sf::Vector2f>(mouseMoved->position);
        const auto wasHovering = m_isHovering;
        m_isHovering = checkCollision(position, view);
        if (m_isHovering && !wasHovering) {
            m_eventSystem.publish(ButtonEvent{
                .buttonName = m_text.getString(),
                .eventType = ButtonEvent::Type::Hovered,
            });
        } else if (wasHovering && !m_isHovering) {
            m_eventSystem.publish(ButtonEvent{
                .buttonName = m_text.getString(),
                .eventType = ButtonEvent::Type::Unhovered,
            });
        }
    }
    if (const auto mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (sf::Mouse::Button::Left == mouseReleased->button) {
            const auto position = static_cast<sf::Vector2f>(mouseReleased->position);
            m_isSelected = checkCollision(position, view);
            if (m_isSelected) {
                m_eventSystem.publish(ButtonEvent{
                    .buttonName = m_text.getString(),
                    .eventType = ButtonEvent::Type::Clicked,
                });
            }
        }
    }
}

void Button::update(float dt) { m_text.setFillColor(m_isHovering ? sf::Color(255, 200, 200) : sf::Color::White); }

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(m_text); }

} // namespace dd

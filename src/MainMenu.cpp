#include <DanceDance/MainMenu.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace dd {

MainMenu::MainMenu(const sf::Font& font, const sf::VideoMode& videoMode) : m_title(font, "Dance Dance", 150), m_videoMode(videoMode) {}

void MainMenu::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::MouseMoved>()) {
        const auto& mousePos = event.getIf<sf::Event::MouseMoved>()->position;
        m_isHovered = m_title.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }
}

void MainMenu::update(float dt) {
    constexpr auto scaleLerpSpeed = 0.05f;
    constexpr auto colorLerpSpeed = 0.05f;

    const auto targetScale = m_isHovered ? 1.2f : 1.0f;
    const auto currentScale = m_title.getScale().x;
    const auto newScale = currentScale + (targetScale - currentScale) * (1.0f - std::exp(-scaleLerpSpeed * dt * 60.0f));
    const auto bounds = m_title.getLocalBounds();

    const auto currentColor = m_title.getFillColor();
    const auto targetColor = m_isHovered ? sf::Color(255, 200, 200) : sf::Color::White;
    const auto lerpFactor = 1.0f - std::exp(-colorLerpSpeed * dt * 60.0f);

    m_title.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    m_title.setScale({newScale, newScale});
    m_title.setPosition({m_videoMode.size.x / 2.f, m_videoMode.size.y / 10.f});
    m_title.setFillColor(sf::Color(currentColor.r + (targetColor.r - currentColor.r) * lerpFactor,
                                   currentColor.g + (targetColor.g - currentColor.g) * lerpFactor,
                                   currentColor.b + (targetColor.b - currentColor.b) * lerpFactor));
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(m_title, states); }

} // namespace dd
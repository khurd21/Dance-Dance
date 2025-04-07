#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <DanceDance/EventSystem.hpp>
#include <DanceDance/GameEvents.hpp>
#include <DanceDance/MainMenu.hpp>

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

namespace dd {

MainMenu::MainMenu(const sf::Font& font, const sf::VideoMode& videoMode, EventSystem& eventSystem)
    : m_title(font, "Dance Dance", 150), m_videoMode(videoMode), m_eventSystem(eventSystem) {
    applySettingsOptions(font);
}

void MainMenu::handleEvent(const sf::Event& event) {
    std::ranges::for_each(m_options, [&](auto& option) { option.handleEvent(event); });
    if (event.is<sf::Event::MouseMoved>()) {
        const auto& mousePos = event.getIf<sf::Event::MouseMoved>()->position;
        m_isHovered = m_title.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }
}

void MainMenu::update(float dt) {
    std::ranges::for_each(m_options, [&](auto& option) { option.update(dt); });

    constexpr auto scaleLerpSpeed = 3.f;
    constexpr auto colorLerpSpeed = 3.f;

    const auto targetScale = m_isHovered ? 1.2f : 1.0f;
    const auto currentScale = m_title.getScale().x;
    const auto newScale = currentScale + (targetScale - currentScale) * (1.0f - std::exp(-scaleLerpSpeed * dt));
    const auto bounds = m_title.getLocalBounds();

    const auto currentColor = m_title.getFillColor();
    const auto targetColor = m_isHovered ? sf::Color(255, 200, 200) : sf::Color::White;
    const auto lerpFactor = 1.0f - std::exp(-colorLerpSpeed * dt);

    m_title.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    m_title.setScale({newScale, newScale});
    m_title.setPosition({m_videoMode.size.x / 2.f, m_videoMode.size.y / 10.f});
    m_title.setFillColor(sf::Color(currentColor.r + (targetColor.r - currentColor.r) * lerpFactor,
                                   currentColor.g + (targetColor.g - currentColor.g) * lerpFactor,
                                   currentColor.b + (targetColor.b - currentColor.b) * lerpFactor));
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    std::ranges::for_each(m_options, [&](const auto& option) { target.draw(option, states); });
    target.draw(m_title, states);
}

void MainMenu::applySettingsOptions(const sf::Font& font) {
    constexpr auto buttonSpacing = 70.f;
    const auto buttonStartPosition = m_videoMode.size.y * .5f;

    m_options.push_back(Button("Play", font, {m_videoMode.size.x / 2.f, buttonStartPosition + (0 * buttonSpacing)}, m_eventSystem));
    m_options.push_back(Button("Exit", font, {m_videoMode.size.x / 2.f, buttonStartPosition + (1 * buttonSpacing)}, m_eventSystem));
    m_eventSystem.subscribe<ButtonClickedEvent>([this](const auto& event) {
        const auto& name = event.buttonName;
        if ("Play" == name) {
            // TODO: Display the song options. When that is approved start the game.
            // TODO: Probably update this view by setting a member variable and then updating the text that is visible.
            // TODO: Once the now visible text is selected and button selected, then publish event with the data about the
            // TODO: Chosen song.
            m_eventSystem.publish(GameStateChangeEvent{.from = GameState::Home, .to = GameState::Play});
        } else if ("Exit" == name) {
            m_eventSystem.publish(ExitGameEvent());
        }
    });
}

} // namespace dd
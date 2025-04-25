#include <DanceDance/Arrow.hpp>
#include <DanceDance/EventSystem.hpp>
#include <DanceDance/Game.hpp>
#include <DanceDance/GameEvents.hpp>
#include <DanceDance/Tape.hpp>

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
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

sf::Vector2f getPosition(const sf::VideoMode& videoMode, dd::Arrow::Direction direction) {
    using Direction = dd::Arrow::Direction;
    const std::unordered_map<Direction, int> offsets{
        {Direction::Left, 0},
        {Direction::Up, 1},
        {Direction::Down, 2},
        {Direction::Right, 3},
    };
    if (!offsets.contains(direction)) {
        return {};
    }

    const auto arrowCount = static_cast<int>(offsets.size());
    const auto y = videoMode.size.y / 6.f;
    const auto centerX = videoMode.size.x / 2.f;
    const auto totalWidth = videoMode.size.x * 0.3f;
    const auto spacing = totalWidth / (arrowCount - 1);
    const auto offset = offsets.at(direction);
    return {centerX - totalWidth / 2.f + offsets.at(direction) * spacing, y};
}

} // namespace

namespace dd {

struct Game::MovingArrow {
    Arrow arrow;
    float startPosition{};
    float endPosition{};
    float elapsed{};
    float travelTime{};
    bool isFading{false};
    float alpha{255.f};
};

Game::Game(const sf::VideoMode& videoMode, EventSystem& eventSystem)
    : m_defaultTexture("assets/images/back-button-white-32x32.png"), m_backButton(m_defaultTexture), m_eventSystem(eventSystem),
      m_videoMode(videoMode) {
    m_defaultTexture.setSmooth(true);
    m_backButton.setPosition({20.f, 20.f});
    m_backButton.setScale({1.2f, 1.2f});

    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Left, getPosition(m_videoMode, Arrow::Direction::Left)));
    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Up, getPosition(m_videoMode, Arrow::Direction::Up)));
    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Down, getPosition(m_videoMode, Arrow::Direction::Down)));
    m_stationaryArrows.push_back(Arrow(Arrow::Direction::Right, getPosition(m_videoMode, Arrow::Direction::Right)));
    m_eventSystem.subscribe<TapeLoadedEvent>([this](const TapeLoadedEvent& tapeLoadedEvent) {
        m_tape = tapeLoadedEvent.tape;
        m_songTime = 0.f;
    });
}

Game::~Game() = default;

void Game::handleEvent(const sf::Event& event) {

    constexpr auto perfectWindow = 0.05f;
    constexpr auto goodWindow = 0.1f;
    constexpr auto missWindow = 0.15f;

    if (const auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_isHovering = m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseMoved->position));
    }
    if (const auto mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseReleased->position))) {
            m_eventSystem.publish(GameStateChangeEvent{.from = GameState::Play, .to = GameState::Home});
        }
    }
}

void Game::update(float dt) {
    m_backButton.setColor(m_isHovering ? sf::Color(255, 200, 200) : sf::Color::White);
    if (!m_tape.has_value()) {
        return;
    }
    m_songTime += dt;
    const auto secondsPerSixteenth = 60.f / m_tape->getBPM() / 4.f;
    while (m_songTime >= m_nextFrameTime) {
        if (auto frame = m_tape->getNextFrame()) {
            if (frame->down) {
                auto position = getPosition(m_videoMode, Arrow::Direction::Down);
                position.y = m_videoMode.size.y + 100.f;
                m_movingArrows.emplace_back(Arrow::Direction::Down, position, true);
            }
            if (frame->left) {
                auto position = getPosition(m_videoMode, Arrow::Direction::Left);
                position.y = m_videoMode.size.y + 100.f;
                m_movingArrows.emplace_back(Arrow::Direction::Left, position, true);
            }
            if (frame->right) {
                auto position = getPosition(m_videoMode, Arrow::Direction::Right);
                position.y = m_videoMode.size.y + 100.f;
                m_movingArrows.emplace_back(Arrow::Direction::Right, position, true);
            }
            if (frame->up) {
                auto position = getPosition(m_videoMode, Arrow::Direction::Up);
                position.y = m_videoMode.size.y + 100.f;
                m_movingArrows.emplace_back(Arrow::Direction::Up, position, true);
            }
            m_nextFrameTime += secondsPerSixteenth;
        } else {
            break;
        }
    }

    // TODO: Update moving arrows
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    std::ranges::for_each(m_stationaryArrows, [&](const auto& arrow) { target.draw(arrow, states); });
    std::ranges::for_each(m_movingArrows, [&](const auto& arrow) { target.draw(arrow, states); });
    target.draw(m_backButton, states);
}

void Game::spawnArrows(const dd::Tape::Frame& frame) {}

} // namespace dd
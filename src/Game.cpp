#include <DanceDance/Arrow.hpp>
#include <DanceDance/EventSystem.hpp>
#include <DanceDance/Game.hpp>
#include <DanceDance/GameEvents.hpp>
#include <DanceDance/Tape.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/System/Clock.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <cstdint>

namespace {

sf::Texture defaultTexture = []() {
    auto texture = sf::Texture("assets/images/back-button-white-32x32.png");
    texture.setSmooth(true);
    return texture;
}();

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
    return {centerX - totalWidth / 2.f + offset * spacing, y};
}

constexpr auto perfectWindow = 20.f;
constexpr auto goodWindow = 50.f;
constexpr auto missWindow = 100.f;

constexpr auto perfectScore = 50;
constexpr auto goodScore = 10;
constexpr auto missScore = 0;
constexpr auto multiplierIncrease = 15;
constexpr auto multiplierMaximum = 5;

constexpr auto judgeTextFadeDuration = 1.5f;
constexpr auto songFadingDuration = 3.f;

} // namespace

namespace dd {

Game::Game(const sf::Font& font, const sf::VideoMode& videoMode, EventSystem& eventSystem)
    : m_scoreText(font, "0", 80), m_multiplierText(font, "x0", 80), m_judgeText(font, "", 80), m_backButton(defaultTexture),
      m_eventSystem(eventSystem), m_videoMode(videoMode) {
    m_backButton.setPosition({20.f, 20.f});
    m_backButton.setScale({1.2f, 1.2f});
    m_scoreText.setOrigin({m_scoreText.getLocalBounds().size.x, 0.f});
    m_scoreText.setPosition({videoMode.size.x / 1.1f, 20.f});
    m_multiplierText.setOrigin({m_multiplierText.getLocalBounds().size.x, 0.f});
    m_multiplierText.setPosition({m_scoreText.getPosition().x, m_scoreText.getPosition().y + m_scoreText.getLocalBounds().size.y + 5.f});
    m_judgeText.setPosition({videoMode.size.x / 8.f, m_backButton.getLocalBounds().size.y + 20.f});

    m_stationaryArrows.emplace_back(Arrow::Direction::Left, getPosition(m_videoMode, Arrow::Direction::Left));
    m_stationaryArrows.emplace_back(Arrow::Direction::Up, getPosition(m_videoMode, Arrow::Direction::Up));
    m_stationaryArrows.emplace_back(Arrow::Direction::Down, getPosition(m_videoMode, Arrow::Direction::Down));
    m_stationaryArrows.emplace_back(Arrow::Direction::Right, getPosition(m_videoMode, Arrow::Direction::Right));
    m_eventSystem.subscribe<TapeLoadedEvent>([this](const TapeLoadedEvent& tapeLoadedEvent) {
        m_tape = tapeLoadedEvent.tape;
        m_music = sf::Music(tapeLoadedEvent.audioPath);
    });
}

Game::~Game() = default;

void Game::handleEvent(const sf::Event& event, sf::View*) {
    handleMouseMoved(event);
    handleMousePressed(event);
    handleButtonPressed(event);
}

void Game::update(float dt) {
    m_backButton.setColor(m_isHovering ? sf::Color(255, 200, 200) : sf::Color::White);
    m_scoreText.setString(std::to_string(m_score));
    m_multiplierText.setString(std::format("x{}", m_multiplier));
    if (!m_tape.has_value() || !m_music.has_value()) {
        return;
    }

    if (m_music->getStatus() == sf::SoundSource::Status::Stopped && !m_hasStarted) {
        m_hasStarted = true;
        m_music->play();
    }

    m_songTime += dt;
    constexpr auto arrowSpeed = 250.f;
    const auto secondsPerSixteenth = 60.f / m_tape->getBPM() / 4.f;
    while (m_songTime >= m_nextFrameTime) {
        if (!m_tape->hasNextFrame()) {
            break;
        }
        const auto frame = m_tape->getNextFrame();
        const auto spawnArrow = [this](bool shouldSpawn, Arrow::Direction direction) {
            if (!shouldSpawn) {
                return;
            }
            auto position = getPosition(m_videoMode, direction);
            position.y = m_videoMode.size.y + 50.f;
            m_movingArrows.emplace_back(direction, position, true);
        };
        spawnArrow(frame->down, Arrow::Direction::Down);
        spawnArrow(frame->left, Arrow::Direction::Left);
        spawnArrow(frame->right, Arrow::Direction::Right);
        spawnArrow(frame->up, Arrow::Direction::Up);
        m_nextFrameTime += secondsPerSixteenth;
    }

    std::ranges::for_each(m_movingArrows, [&](auto& arrow) { arrow.move({0.f, -arrowSpeed * dt}); });
    const auto previousSize{m_movingArrows.size()};
    m_movingArrows.erase(std::remove_if(m_movingArrows.begin(), m_movingArrows.end(),
                                        [this](const auto& arrow) {
                                            const auto arrowY = arrow.getPosition().y;
                                            const auto stationaryY = m_stationaryArrows.at(0).getPosition().y;
                                            return arrowY < stationaryY - missWindow;
                                        }),
                         m_movingArrows.end());
    const auto currentSize{m_movingArrows.size()};
    if (currentSize != previousSize) {
        handleRating(Rating::Miss);
    }

    if (m_isJudging) {
        const auto elapsed = m_judgeTextClock.getElapsedTime().asSeconds();
        if (elapsed >= judgeTextFadeDuration) {
            m_judgeText.setString("");
            m_isJudging = false;
        } else {
            auto color = m_judgeText.getFillColor();
            const auto alpha = 255 * (1.0f - (elapsed / judgeTextFadeDuration));
            color.a = static_cast<std::uint8_t>(alpha);
            m_judgeText.setFillColor(color);
        }
    }

    if (previousSize == 0 && !m_tape->hasNextFrame()) {
        if (!m_isSongFading) {
            m_isSongFading = !m_isSongFading;
            m_initialVolume = m_music->getVolume();
            m_songFadingClock.restart();
        }
        const auto elapsed = m_songFadingClock.getElapsedTime().asSeconds();
        if (elapsed >= songFadingDuration) {
            reset();
            m_eventSystem.publish(GameStateChangeEvent{.from = GameState::Play, .to = GameState::Home});
        } else {
            const auto progress = elapsed / songFadingDuration;
            const auto newVolume = m_initialVolume * (1.f - progress);
            m_music->setVolume(newVolume);
        }
    }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    std::ranges::for_each(m_stationaryArrows, [&](const auto& arrow) { target.draw(arrow, states); });
    std::ranges::for_each(m_movingArrows, [&](const auto& arrow) { target.draw(arrow, states); });
    target.draw(m_scoreText, states);
    target.draw(m_multiplierText, states);
    target.draw(m_judgeText, states);
    target.draw(m_backButton, states);
}

void Game::handleMouseMoved(const sf::Event& event) {
    if (const auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_isHovering = m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseMoved->position));
    }
}

void Game::handleMousePressed(const sf::Event& event) {
    if (const auto mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseReleased->position))) {
            reset();
            m_eventSystem.publish(GameStateChangeEvent{.from = GameState::Play, .to = GameState::Home});
        }
    }
}

void Game::handleRating(Rating rating) {
    constexpr std::array validRatings{Rating::Miss, Rating::Good, Rating::Perfect};
    if (std::ranges::find(validRatings, rating) == validRatings.cend()) {
        return;
    }

    m_isJudging = true;
    m_judgeTextClock.restart();
    if (Rating::Miss == rating) {
        m_streak = 0;
        m_multiplier = 1;
        m_score += missScore * m_multiplier;
        m_judgeText.setString("MISS");
        m_judgeText.setFillColor(sf::Color::Red);
    } else if (Rating::Good == rating) {
        ++m_streak;
        m_multiplier += static_cast<int>(m_streak % multiplierIncrease == 0);
        m_multiplier = std::min(multiplierMaximum, m_multiplier);
        m_score += goodScore * m_multiplier;
        m_judgeText.setString("GOOD");
        m_judgeText.setFillColor(sf::Color::Yellow);
    } else if (Rating::Perfect == rating) {
        ++m_streak;
        m_multiplier += static_cast<int>(m_streak % multiplierIncrease == 0);
        m_multiplier = std::min(multiplierMaximum, m_multiplier);
        m_score += perfectScore * m_multiplier;
        m_judgeText.setString("PERFECT");
        m_judgeText.setFillColor(sf::Color::Green);
    }
}

void Game::reset() {
    m_music = std::nullopt;
    m_tape = std::nullopt;
    m_judgeText.setString({});
    m_hasStarted = {};
    m_isJudging = {};
    m_isSongFading = {};
    m_songTime = {};
    m_nextFrameTime = {};
    m_score = {};
    m_movingArrows.clear();
    m_judgeTextClock.restart();
}

void Game::handleButtonPressed(const sf::Event& event) {
    const std::unordered_map<sf::Keyboard::Key, Arrow::Direction> keys{
        {sf::Keyboard::Key::Left, Arrow::Direction::Left},
        {sf::Keyboard::Key::Up, Arrow::Direction::Up},
        {sf::Keyboard::Key::Down, Arrow::Direction::Down},
        {sf::Keyboard::Key::Right, Arrow::Direction::Right},
    };

    if (const auto buttonPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (!keys.contains(buttonPressed->code)) {
            return;
        }
        const auto& arrow = m_stationaryArrows.at(static_cast<std::underlying_type_t<Arrow::Direction>>(keys.at(buttonPressed->code)));
        auto iter = std::ranges::min_element(m_movingArrows, {}, [&](const auto& arrow) {
            if (arrow.getDirection() != keys.at(buttonPressed->code)) {
                return std::numeric_limits<float>::max();
            }
            return std::abs(arrow.getPosition().y - arrow.getPosition().y);
        });
        if (iter == m_movingArrows.cend() || arrow.getDirection() != iter->getDirection()) {
            return;
        }
        const auto distance = std::abs(iter->getPosition().y - arrow.getPosition().y);
        m_movingArrows.erase(iter);
        if (distance <= perfectWindow) {
            handleRating(Rating::Perfect);
        } else if (distance <= goodWindow) {
            handleRating(Rating::Good);
        } else if (distance <= missWindow) {
            handleRating(Rating::Miss);
        }
    }
}

} // namespace dd
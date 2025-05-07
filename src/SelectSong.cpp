#include <DanceDance/SelectSong.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include <DanceDance/Button.hpp>
#include <DanceDance/GameEvents.hpp>
#include <DanceDance/SongFinder.hpp>

#include <algorithm>

namespace {

sf::Texture backButtonTexture = []() {
    auto texture = sf::Texture("assets/images/back-button-white-32x32.png");
    texture.setSmooth(true);
    return texture;
}();

} // namespace

namespace dd {

SelectSong::SelectSong(const sf::Font& font, const sf::VideoMode& videoMode, EventSystem& eventSystem)
    : m_title(font, "Select a Song", 150), m_backButton(backButtonTexture),
      m_scrollableView(sf::FloatRect({0.f, 0.f}, static_cast<sf::Vector2f>(videoMode.size))), m_videoMode(videoMode), m_eventSystem(eventSystem) {

    const auto bounds{m_title.getLocalBounds()};
    m_backButton.setPosition({20.f, 20.f});
    m_backButton.setScale({1.2f, 1.2f});
    m_title.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    m_title.setPosition({m_videoMode.size.x / 2.f, m_videoMode.size.y / 10.f});

    const auto scrollAreaTop = m_videoMode.size.y / 10.f + bounds.size.y + 50.f;
    const auto scrollAreaHeight = m_videoMode.size.y - scrollAreaTop - 50.f;
    const auto scrollAreaWidth = static_cast<float>(m_videoMode.size.x);
    const auto scrollAreaLeft = (m_videoMode.size.x - scrollAreaWidth) / 2.f;
    m_scrollableView.setViewport(sf::FloatRect({scrollAreaLeft / m_videoMode.size.x, scrollAreaTop / m_videoMode.size.y},
                                               {scrollAreaWidth / m_videoMode.size.x, scrollAreaHeight / m_videoMode.size.y}));

    auto contentHeight = 0.f;
    const auto buttonWidth = scrollAreaWidth * 0.9f;
    constexpr auto buttonSpacing = 20.f;
    constexpr auto buttonHeight = 60.f;
    const auto& availableSongs = SongFinder::GetSongFinder().getFoundSongs();
    std::ranges::for_each(availableSongs, [&](const auto& song) {
        m_songs.emplace_back(Button(song.title, font, {scrollAreaWidth / 2.f, contentHeight + buttonHeight / 2.f}, m_eventSystem));
        m_eventSystem.subscribe<ButtonClickedEvent>([&](const auto& event) {
            if (song.title == event.buttonName) {
                m_eventSystem.publish<TapeLoadedEvent>(TapeLoadedEvent{.tape = Tape(song.tapePath)});
                m_eventSystem.publish<GameStateChangeEvent>(GameStateChangeEvent{
                    .from = GameState::SelectSong,
                    .to = GameState::Play,
                });
            }
        });
        contentHeight += buttonHeight + buttonSpacing;
    });

    m_scrollContentHeight = contentHeight;
    m_currentScrollPosition = 0.f;
    m_maxScrollPosition = std::max(0.f, contentHeight - scrollAreaHeight);
}

void SelectSong::handleEvent(const sf::Event& event, sf::View*) {
    std::ranges::for_each(m_songs, [&](auto& song) { song.handleEvent(event, &m_scrollableView); });
    if (const auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_isBackButtonHovered = m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseMoved->position));
    }
    if (const auto mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (m_backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseReleased->position))) {
            m_eventSystem.publish(GameStateChangeEvent{
                .from = GameState::SelectSong,
                .to = GameState::Home,
            });
        }
    }
}

void SelectSong::update(float dt) {
    std::ranges::for_each(m_songs, [&](auto& song) { song.update(dt); });
    m_backButton.setColor(m_isBackButtonHovered ? sf::Color(255, 200, 200) : sf::Color::White);
}

void SelectSong::handleScroll(float dt) {
    m_currentScrollPosition = std::clamp(m_currentScrollPosition - dt * 30.f, 0.f, m_maxScrollPosition);
    m_scrollableView.setCenter({m_scrollableView.getSize().x / 2.f, m_scrollableView.getSize().y / 2.f + m_currentScrollPosition});
}

void SelectSong::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_title, states);
    target.draw(m_backButton, states);

    auto oldView = target.getView();
    target.setView(m_scrollableView);
    sf::RectangleShape scrollBackground(m_scrollableView.getSize());
    scrollBackground.setPosition({0.f, 0.f});
    scrollBackground.setFillColor(sf::Color::Black);
    target.draw(scrollBackground);
    std::ranges::for_each(m_songs, [&](const auto& song) { target.draw(song, states); });
    target.setView(oldView);
}

} // namespace dd
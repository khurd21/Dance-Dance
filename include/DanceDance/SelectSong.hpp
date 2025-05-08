#ifndef DANCE_DANCE_SELECT_SONG_HPP
#define DANCE_DANCE_SELECT_SONG_HPP

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <DanceDance/Button.hpp>
#include <DanceDance/EventSystem.hpp>
#include <DanceDance/GameEvents.hpp>
#include <DanceDance/IScene.hpp>
#include <DanceDance/SongFinder.hpp>

#include <optional>
#include <vector>

namespace dd {

class SelectSong : public IScene {
  public:
    explicit SelectSong(const sf::Font& font, const sf::VideoMode& videoMode, EventSystem& eventSystem);
    void handleEvent(const sf::Event& event, sf::View* = nullptr) override;
    void update(float dt) override;

  private:
    void handleScroll(float dt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void onButtonHoveredEvent(const SongFinder::Song& song, const ButtonEvent& event);
    void onButtonUnhoveredEvent(const SongFinder::Song& song, const ButtonEvent& event);
    void onButtonClickedEvent(const SongFinder::Song& song, const ButtonEvent& event);

    sf::Text m_title;
    sf::Sprite m_backButton;
    sf::View m_scrollableView;
    sf::VideoMode m_videoMode;
    std::vector<Button> m_songs;
    EventSystem& m_eventSystem;

    bool m_isBackButtonHovered{};
    float m_scrollContentHeight{};
    float m_currentScrollPosition{};
    float m_maxScrollPosition{};
    std::optional<sf::Music> m_currentPreview;
};

} // namespace dd

#endif // DANCE_DANCE_SELECT_SONG_HPP
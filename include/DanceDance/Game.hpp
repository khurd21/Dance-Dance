#ifndef DANCE_DANCE_GAME_HPP
#define DANCE_DANCE_GAME_HPP

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>

#include <DanceDance/Arrow.hpp>
#include <DanceDance/EventSystem.hpp>
#include <DanceDance/IScene.hpp>
#include <DanceDance/Tape.hpp>

#include <filesystem>
#include <optional>
#include <vector>

namespace dd {

class Game : public IScene {
  public:
    explicit Game(const sf::Font& font, const sf::VideoMode& videoMode, EventSystem& eventSystem);
    ~Game();
    void handleEvent(const sf::Event& event, sf::View* = nullptr) override;
    void update(float dt) override;

  private:
    enum class Rating {
        Miss,
        Good,
        Perfect,
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void handleRating(Rating rating);
    void reset();

    void handleMouseMoved(const sf::Event& event);
    void handleMousePressed(const sf::Event& event);
    void handleButtonPressed(const sf::Event& event);

    sf::Text m_scoreText;
    sf::Text m_multiplierText;
    sf::Text m_judgeText;
    sf::Sprite m_backButton;
    sf::VideoMode m_videoMode;
    sf::Clock m_judgeTextClock;
    sf::Clock m_songFadingClock;

    std::optional<Tape> m_tape;
    std::optional<sf::Music> m_music;
    float m_songTime{};
    float m_nextFrameTime{};
    float m_initialVolume{};
    int m_score{};
    int m_multiplier{1};
    int m_streak{};

    bool m_hasStarted{};
    bool m_isHovering{};
    bool m_isJudging{};
    bool m_isSongFading{};

    std::vector<Arrow> m_stationaryArrows;
    std::vector<Arrow> m_movingArrows;
    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_GAME_HPP
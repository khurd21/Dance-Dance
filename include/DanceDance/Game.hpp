#ifndef DANCE_DANCE_GAME_HPP
#define DANCE_DANCE_GAME_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

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
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void handleMouseMoved(const sf::Event& event);
    void handleMousePressed(const sf::Event& event);
    void handleButtonPressed(const sf::Event& event);

    sf::Text m_scoreText;
    sf::Sprite m_backButton;
    sf::VideoMode m_videoMode;

    std::optional<Tape> m_tape;
    float m_songTime{};
    float m_nextFrameTime{};
    int m_score{};

    bool m_isHovering{};
    std::vector<Arrow> m_stationaryArrows;
    std::vector<Arrow> m_movingArrows;

    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_GAME_HPP
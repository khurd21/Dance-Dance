#ifndef DANCE_DANCE_GAME_HPP
#define DANCE_DANCE_GAME_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
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
    explicit Game(const sf::VideoMode& videoMode, EventSystem& eventSystem);
    ~Game();
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void spawnArrows(const dd::Tape::Frame& frame);

    // Forward declare for additional properties of Arrow
    struct MovingArrow;

    sf::Texture m_defaultTexture;
    sf::Texture m_onHoverTexture;
    sf::Sprite m_backButton;
    sf::VideoMode m_videoMode;

    std::optional<Tape> m_tape;
    float m_songTime{};
    float m_nextFrameTime{};

    bool m_isHovering{};
    std::vector<Arrow> m_stationaryArrows;
    std::vector<Arrow> m_movingArrows;

    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_GAME_HPP
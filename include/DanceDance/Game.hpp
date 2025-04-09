#ifndef DANCE_DANCE_GAME_HPP
#define DANCE_DANCE_GAME_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <DanceDance/Arrow.hpp>
#include <DanceDance/EventSystem.hpp>
#include <DanceDance/IScene.hpp>

#include <filesystem>
#include <vector>

namespace dd {

class Game : public IScene {
  public:
    explicit Game(const sf::VideoMode& videoMode, EventSystem& eventSystem);
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Texture m_defaultTexture;
    sf::Texture m_onHoverTexture;
    sf::Sprite m_backButton;
    sf::VideoMode m_videoMode;

    bool m_isHovering{};
    std::vector<Arrow> m_stationaryArrows;

    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_GAME_HPP
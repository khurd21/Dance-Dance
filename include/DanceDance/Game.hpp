#ifndef DANCE_DANCE_GAME_HPP
#define DANCE_DANCE_GAME_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <DanceDance/EventSystem.hpp>
#include <DanceDance/IScene.hpp>

#include <filesystem>

namespace dd {

class Game : public IScene {
  public:
    explicit Game(EventSystem& eventSystem);
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Texture m_defaultTexture;
    sf::Texture m_onHoverTexture;
    bool m_isHovering{};

    sf::Sprite m_backButton;
    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_GAME_HPP
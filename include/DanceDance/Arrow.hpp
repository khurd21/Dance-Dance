#ifndef DANCE_DANCE_ARROW_HPP
#define DANCE_DANCE_ARROW_HPP

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <DanceDance/IScene.hpp>

namespace dd {

class Arrow : public IScene {
  public:
    enum class Direction { Left, Up, Down, Right };

    explicit Arrow(Direction direction, const sf::Vector2f& position);
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_sprite;
};

} // namespace dd

#endif // DANCE_DANCE_ARROW_HPP
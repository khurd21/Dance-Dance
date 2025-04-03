#ifndef DANCE_DANCE_BUTTON_HPP
#define DANCE_DANCE_BUTTON_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include <DanceDance/IScene.hpp>

#include <string>

namespace dd {

class Button : public IScene {
  public:
    explicit Button(const std::string& label, const sf::Font& font, const sf::Vector2f& position);
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_rectangleShape;
    sf::Text m_text;
    bool m_isHovering{};
    bool m_isSelected{};
};

} // namespace dd

#endif // DANCE_DANCE_BUTTON_HPP
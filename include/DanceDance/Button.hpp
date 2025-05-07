#ifndef DANCE_DANCE_BUTTON_HPP
#define DANCE_DANCE_BUTTON_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>

#include <string>

#include <DanceDance/EventSystem.hpp>
#include <DanceDance/IScene.hpp>

namespace dd {

class Button : public IScene {
  public:
    explicit Button(const std::string& label, const sf::Font& font, const sf::Vector2f& position, EventSystem& eventSystem);
    void handleEvent(const sf::Event& event, sf::View* view = nullptr) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Text m_text;
    bool m_isHovering{};
    bool m_isSelected{};
    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_BUTTON_HPP
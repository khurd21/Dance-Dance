#ifndef DANCE_DANCE_MAIN_MENU_HPP
#define DANCE_DANCE_MAIN_MENU_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <DanceDance/IScene.hpp>

namespace dd {

class MainMenu : public IScene {
  public:
    explicit MainMenu(const sf::Font& font, const sf::VideoMode& videoMode);
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Text m_title;
    sf::VideoMode m_videoMode;
    bool m_isHovered{};
};

} // namespace dd

#endif // DANCE_DANCE_MAIN_MENU_HPP

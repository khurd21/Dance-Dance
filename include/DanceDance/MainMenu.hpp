#ifndef DANCE_DANCE_MAIN_MENU_HPP
#define DANCE_DANCE_MAIN_MENU_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <DanceDance/Button.hpp>
#include <DanceDance/EventSystem.hpp>
#include <DanceDance/IScene.hpp>

#include <vector>

namespace dd {

class MainMenu : public IScene {
  public:
    explicit MainMenu(const sf::Font& font, const sf::VideoMode& videoMode, EventSystem& eventSystem);
    void handleEvent(const sf::Event& event, sf::View* = nullptr) override;
    void update(float dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void applySettingsOptions(const sf::Font& font);

    sf::Text m_title;
    sf::VideoMode m_videoMode;
    std::vector<Button> m_options;
    bool m_isHovered{};
    EventSystem& m_eventSystem;
};

} // namespace dd

#endif // DANCE_DANCE_MAIN_MENU_HPP

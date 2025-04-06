#ifndef DANCE_DANCE_RUNNER_HPP
#define DANCE_DANCE_RUNNER_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include <unordered_map>

#include <DanceDance/EventSystem.hpp>
#include <DanceDance/GameState.hpp>
#include <DanceDance/IScene.hpp>

namespace dd {

class Runner {
  public:
    explicit Runner();
    void run();

  private:
    sf::Clock m_clock;
    sf::Font m_font;
    sf::RenderWindow m_renderWindow;
    GameState m_currentState = GameState::Home;
    EventSystem m_eventSystem;
    std::unordered_map<GameState, std::unique_ptr<IScene>> m_states;
};

} // namespace dd

#endif // DANCE_DANCE_RUNNER_HPP
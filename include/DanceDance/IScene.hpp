#ifndef DANCE_DANCE_ISCENE_HPP
#define DANCE_DANCE_ISCENE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>

namespace dd {

class IScene : public sf::Drawable {
  public:
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
};

} // namespace dd

#endif // DANCE_DANCE_ISCENE_HPP

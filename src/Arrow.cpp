#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <DanceDance/Arrow.hpp>
#include <DanceDance/SVGTexture.hpp>

#include <unordered_map>

namespace {

dd::SVGTexture arrowTexture("assets/images/left-arrow-outline.svg");
dd::SVGTexture arrowFilledTexture("assets/images/left-arrow-filled.svg");

} // namespace

namespace dd {

Arrow::Arrow(Direction direction, const sf::Vector2f& position, bool isFilled)
    : m_sprite(isFilled ? arrowFilledTexture.getTexture() : arrowTexture.getTexture()) {
    std::unordered_map<Direction, sf::Angle> degrees{
        {Direction::Left, sf::degrees(0.f)},
        {Direction::Up, sf::degrees(90.f)},
        {Direction::Down, sf::degrees(-90.f)},
        {Direction::Right, sf::degrees(180.f)},
    };
    arrowTexture.setSmooth(true);
    m_sprite.setScale({0.2f, 0.2f});
    m_sprite.setOrigin({m_sprite.getLocalBounds().size.x / 2.f, m_sprite.getLocalBounds().size.y / 2.f});
    m_sprite.setPosition(position);
    if (degrees.contains(direction)) {
        m_sprite.setRotation(degrees.at(direction));
    }
}

void Arrow::handleEvent(const sf::Event& event) {}

void Arrow::update(float dt) {}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(m_sprite, states); }

} // namespace dd
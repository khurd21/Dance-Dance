#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <DanceDance/Arrow.hpp>
#include <DanceDance/SVGTexture.hpp>

#include <unordered_map>

namespace {

const auto arrowTexture = []() {
    dd::SVGTexture texture("assets/images/left-arrow-outline.svg");
    texture.setSmooth(true);
    return texture;
}();

const auto arrowFilledTexture = []() {
    dd::SVGTexture texture("assets/images/left-arrow-filled.svg");
    texture.setSmooth(true);
    return texture;
}();

} // namespace

namespace dd {

Arrow::Arrow(Direction direction, const sf::Vector2f& position, bool isFilled)
    : m_sprite(isFilled ? arrowFilledTexture.getTexture() : arrowTexture.getTexture()), m_direction(direction) {
    std::unordered_map<Direction, sf::Angle> degrees{
        {Direction::Left, sf::degrees(0.f)},
        {Direction::Up, sf::degrees(90.f)},
        {Direction::Down, sf::degrees(-90.f)},
        {Direction::Right, sf::degrees(180.f)},
    };
    m_sprite.setScale({0.2f, 0.2f});
    m_sprite.setOrigin({m_sprite.getLocalBounds().size.x / 2.f, m_sprite.getLocalBounds().size.y / 2.f});
    m_sprite.setPosition(position);
    if (degrees.contains(direction)) {
        m_sprite.setRotation(degrees.at(direction));
    }
}

void Arrow::handleEvent(const sf::Event& event, sf::View*) {}

void Arrow::update(float dt) {}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(m_sprite, states); }

void Arrow::move(sf::Vector2f offset) { m_sprite.move(offset); }

sf::Vector2f Arrow::getPosition() const { return m_sprite.getPosition(); }

Arrow::Direction Arrow::getDirection() const { return m_direction; }

} // namespace dd
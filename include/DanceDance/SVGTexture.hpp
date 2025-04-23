#ifndef DANCE_DANCE_SVG_TEXTURE_HPP
#define DANCE_DANCE_SVG_TEXTURE_HPP

#include <filesystem>
#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace dd {

class SVGTexture {
  public:
    SVGTexture(const std::filesystem::path& path, float scale = 1.f);
    ~SVGTexture();
    SVGTexture& operator=(SVGTexture&) = delete;
    SVGTexture(const SVGTexture&) = delete;

    void setScale(float scale);
    float getScale() const;

    const sf::Texture& getTexture() const;
    sf::Vector2u getSize() const;

    void setSmooth(bool smooth);
    bool isSmooth() const;

  private:
    void rasterize();

    struct SVGData;
    std::unique_ptr<SVGData> m_svgData;
    sf::Texture m_texture;
    float m_scale{1.f};
};

} // namespace dd

#endif // DANCE_DANCE_SVG_TEXTURE_HPP
#include <DanceDance/SVGTexture.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvg.h>
#include <nanosvgrast.h>

namespace dd {

struct SVGTexture::SVGData {
    NSVGimage* image = nullptr;
    NSVGrasterizer* rasterizer = nullptr;

    SVGData() = default;
    ~SVGData() {
        if (image) {
            nsvgDelete(image);
        }
        if (rasterizer) {
            nsvgDeleteRasterizer(rasterizer);
        }
    }

    SVGData(const SVGData&) = delete;
    SVGData& operator=(const SVGData&) = delete;
};

SVGTexture::SVGTexture(const std::filesystem::path& path, float scale) : m_scale(scale) {
    m_svgData = std::make_unique<SVGData>();
    m_svgData->rasterizer = nsvgCreateRasterizer();
    m_svgData->image = nsvgParseFromFile(std::filesystem::absolute(path).string().c_str(), "px", 96);
    rasterize();
}

SVGTexture::~SVGTexture() = default;

void SVGTexture::setScale(float scale) {
    if (scale != m_scale) {
        m_scale = scale;
        rasterize();
    }
}

float SVGTexture::getScale() const { return m_scale; }

const sf::Texture& SVGTexture::getTexture() const { return m_texture; }

sf::Vector2u SVGTexture::getSize() const { return m_texture.getSize(); }

void SVGTexture::setSmooth(bool smooth) { m_texture.setSmooth(smooth); }

bool SVGTexture::isSmooth() const { return m_texture.isSmooth(); }

void SVGTexture::rasterize() {
    if (!m_svgData || !m_svgData->image || !m_svgData->rasterizer) {
        return;
    }

    const auto width{static_cast<unsigned int>(m_svgData->image->width * m_scale)};
    const auto height{static_cast<unsigned int>(m_svgData->image->height * m_scale)};
    std::vector<unsigned char> pixels(width * height * 4);
    nsvgRasterize(m_svgData->rasterizer, m_svgData->image, 0, 0, m_scale, pixels.data(), width, height, width * 4);
    std::ignore = m_texture.resize({width, height});
    m_texture.update(pixels.data());
}

} // namespace dd
#ifndef DANCE_DANCE_TAPE_HPP
#define DANCE_DANCE_TAPE_HPP

#include <array>
#include <filesystem>
#include <optional>
#include <vector>

namespace dd {

class Tape {
  public:
    explicit Tape(const std::filesystem::path& file);
    Tape() = default;

    struct Frame {
        bool left{};
        bool up{};
        bool down{};
        bool right{};
    };

    float getBPM() const;
    std::optional<Frame> getNextFrame();

  private:
    float m_bpm{};
    std::vector<Frame> m_tape;
    std::vector<Frame>::const_iterator m_currentFrame;
};

} // namespace dd

#endif // DANCE_DANCE_TAPE_HPP
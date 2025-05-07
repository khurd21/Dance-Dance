#ifndef DANCE_DANCE_TAPE_HPP
#define DANCE_DANCE_TAPE_HPP

#include <array>
#include <filesystem>
#include <optional>
#include <string>
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
    const std::string& getTitle() const;
    std::optional<Frame> getNextFrame();

  private:
    float m_bpm{};
    std::string m_title;
    std::vector<Frame> m_tape;
    std::size_t m_currentFrameIndex{};
};

} // namespace dd

#endif // DANCE_DANCE_TAPE_HPP
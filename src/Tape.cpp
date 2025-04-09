#include <DanceDance/Tape.hpp>

#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace dd {

Tape::Tape(const std::filesystem::path& file) {
    if (!exists(file)) {
        throw std::runtime_error(std::format("Failed to open file: {}", file.string()));
    }

    std::ifstream stream(file);
    if (!stream.is_open()) {
        throw std::runtime_error(std::format("Failed to open file: {}", file.string()));
    }

    std::string line;
    if (!std::getline(stream, line)) {
        throw std::runtime_error("The tape file is empty.");
    }

    std::istringstream iss(line);
    std::string label;
    if (!(iss >> label >> m_bpm) || label != "BPM:") {
        throw std::runtime_error("Invalid BPM format.");
    }

    while (std::getline(stream, line)) {
        if (line.size() != 4) {
            throw std::runtime_error(std::format("Recieved tape line of size {}. Expected size 4.", line.size()));
        }

        m_tape.push_back(Frame{
            .left = line.at(0) == 'x',
            .up = line.at(1) == 'x',
            .down = line.at(2) == 'x',
            .right = line.at(3) == 'x',
        });
    }
}

float Tape::getBPM() const { return m_bpm; }

std::optional<Tape::Frame> Tape::getNextFrame() {
    if (m_currentFrame >= m_tape.cend()) {
        return std::nullopt;
    }
    return *m_currentFrame++;
}

} // namespace dd
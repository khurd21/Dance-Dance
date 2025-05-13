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

    std::string bpm;
    std::getline(stream, m_title);
    std::getline(stream, bpm);
    m_bpm = std::stof(bpm);

    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            continue;
        }
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

const std::string& Tape::getTitle() const { return m_title; }

std::optional<Tape::Frame> Tape::getNextFrame() {
    if (!hasNextFrame()) {
        return std::nullopt;
    }
    return m_tape.at(m_currentFrameIndex++);
}

bool Tape::hasNextFrame() const {
    return m_currentFrameIndex < m_tape.size();
}

} // namespace dd
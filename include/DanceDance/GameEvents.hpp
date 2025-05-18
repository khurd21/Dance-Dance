#ifndef DANCE_DANCE_GAME_EVENTS_HPP
#define DANCE_DANCE_GAME_EVENTS_HPP

#include <DanceDance/GameState.hpp>
#include <DanceDance/Tape.hpp>

#include <string>
#include <filesystem>

namespace dd {

struct TapeLoadedEvent {
    Tape tape;
    std::filesystem::path audioPath;
};

struct ButtonEvent {
    enum class Type {
        Clicked,
        Hovered,
        Unhovered,
        None,
    };
    std::string buttonName;
    Type eventType{Type::None};
};

struct ExitGameEvent {};

struct GameStateChangeEvent {
    GameState from{};
    GameState to{};
};

} // namespace dd

#endif // DANCE_DANCE_GAME_EVENTS_HPP
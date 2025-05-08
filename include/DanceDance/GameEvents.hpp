#ifndef DANCE_DANCE_GAME_EVENTS_HPP
#define DANCE_DANCE_GAME_EVENTS_HPP

#include <string>

#include <DanceDance/GameState.hpp>
#include <DanceDance/Tape.hpp>

namespace dd {

struct TapeLoadedEvent {
    Tape tape;
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
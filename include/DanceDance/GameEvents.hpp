#ifndef DANCE_DANCE_GAME_EVENTS_HPP
#define DANCE_DANCE_GAME_EVENTS_HPP

#include <string>

#include <DanceDance/GameState.hpp>
#include <DanceDance/Tape.hpp>

namespace dd {

struct TapeLoadedEvent {
    Tape tape;
};

struct ButtonClickedEvent {
    std::string buttonName;
};

struct ExitGameEvent {};

struct GameStateChangeEvent {
    GameState from{};
    GameState to{};
};

} // namespace dd

#endif // DANCE_DANCE_GAME_EVENTS_HPP
#ifndef DANCE_DANCE_GAME_EVENTS_HPP
#define DANCE_DANCE_GAME_EVENTS_HPP

#include <string>

#include <DanceDance/GameState.hpp>

namespace dd {

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
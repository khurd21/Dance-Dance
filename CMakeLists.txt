cmake_minimum_required(VERSION 3.27)
project(DanceDance LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY git@github.com:SFML/SFML.git
    GIT_TAG 3.0.x
)
FetchContent_MakeAvailable(SFML)

set(sources
    src/main.cpp
    src/Arrow.cpp
    src/Button.cpp
    src/MainMenu.cpp
    src/Runner.cpp
    src/Game.cpp
    src/Tape.cpp
)

add_executable(DanceDance ${sources})

target_include_directories(DanceDance PUBLIC include)
target_link_libraries(DanceDance PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio SFML::Network)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
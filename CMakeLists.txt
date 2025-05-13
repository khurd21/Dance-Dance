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

FetchContent_Declare(
    nanosvg
    GIT_REPOSITORY git@github.com:memononen/nanosvg.git
    GIT_TAG master
)
FetchContent_MakeAvailable(nanosvg)

set(sources
    src/main.cpp
    src/Arrow.cpp
    src/Button.cpp
    src/MainMenu.cpp
    src/Runner.cpp
    src/Game.cpp
    src/Tape.cpp
    src/SVGTexture.cpp
    src/SelectSong.cpp
    src/SongFinder.cpp
)

set(includes
    include/DanceDance/Arrow.hpp
    include/DanceDance/Button.hpp
    include/DanceDance/MainMenu.hpp
    include/DanceDance/Runner.hpp
    include/DanceDance/Game.hpp
    include/DanceDance/Tape.hpp
    include/DanceDance/SVGTexture.hpp
    include/DanceDance/SelectSong.hpp
    include/DanceDance/SongFinder.hpp
)

add_executable(DanceDance ${sources} ${includes})

target_include_directories(DanceDance PUBLIC include)
target_include_directories(DanceDance PRIVATE ${nanosvg_SOURCE_DIR}/src)
target_link_libraries(DanceDance PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio SFML::Network)
target_link_libraries(DanceDance PRIVATE nanosvg)

add_custom_command(TARGET DanceDance POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:DanceDance>/assets
    COMMENT "Copying assets to executable directory"
)

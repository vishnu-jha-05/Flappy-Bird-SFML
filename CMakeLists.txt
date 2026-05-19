cmake_minimum_required(VERSION 3.10)
project(FlappyBird)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 3.0 uses capitalized component names
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Bird.cpp
    src/Pipe.cpp
    src/AssetManager.cpp
)

set(HEADERS
    include/Game.h
    include/Bird.h
    include/Pipe.h
    include/AssetManager.h
)

add_executable(FlappyBird ${SOURCES} ${HEADERS})

target_link_libraries(FlappyBird SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Copy assets to build directory for easy execution
add_custom_command(TARGET FlappyBird POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:FlappyBird>/assets")

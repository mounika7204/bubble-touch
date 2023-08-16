cmake_minimum_required(VERSION 3.18)

project(BubbleTouch
        LANGUAGES CXX
        DESCRIPTION "An interactive game where players compete to pop bubbles.")

set(CMAKE_CXX_STANDARD 20)

# System dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
find_package(OpenCV REQUIRED)
find_package(OpenSSL)

# Bundled dependencies
add_subdirectory(third_party/yaml-cpp)

add_executable(bubbletouch
        main.cpp
        initial_window.cpp
        bubbletouch_application.cpp
        game_window.cpp
        game.cpp
        videocamera.cpp
        game_widget.cpp
        player_score_widget.cpp
        mark_detection.cpp
        config.cpp
        bubble.cpp
        player.cpp
        game_over_window.cpp
        particle.cpp
        file_watcher.cpp)

target_compile_options(bubbletouch PRIVATE
        -Wall -Wextra)

target_include_directories(bubbletouch PRIVATE
        ${GTKMM_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS})

target_link_libraries(bubbletouch
        ${GTKMM_LIBRARIES}
        ${OpenCV_LIBS}
        yaml-cpp::yaml-cpp
        OpenSSL::Crypto
        pthread)

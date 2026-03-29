cmake_minimum_required(VERSION 3.30)
project(tuitris)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# ncurses
find_package(Curses REQUIRED)
if(Curses_FOUND AND NOT TARGET Curses::Curses)
  add_library(Curses::Curses INTERFACE IMPORTED)
  set_target_properties(
    Curses::Curses
    PROPERTIES
      INTERFACE_LINK_LIBRARIES "${CURSES_LIBRARIES}"
      INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
  )
endif()

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/015950a936fc6b7cadca20eb0dbf6f054e6637fb.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/colours.cpp
  src/coordinates.cpp
  src/game_window.cpp
  src/inputs.cpp
  src/game/game.cpp
  src/game/game_session.cpp
  src/game/tetromino.cpp
  src/game/tile_grid.cpp
  src/game/tile.cpp
  src/game/tetromino_bag.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE Curses::Curses)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

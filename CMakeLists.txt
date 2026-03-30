cmake_minimum_required(VERSION 3.30)
project(tuitris)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(gamelib
  src/coordinates.cpp
  src/game/game_session.cpp
  src/game/tetromino_bag.cpp
  src/game/tetromino.cpp
  src/game/tile_grid.cpp
  src/game/tile.cpp
)
target_include_directories(gamelib PRIVATE ${INCLUDE_DIR})
target_compile_options(gamelib PRIVATE -Wall)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/tui_colours.cpp
  src/game_window.cpp
  src/inputs.cpp
  src/game/game.cpp
)

find_package(Curses REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE gamelib ${CURSES_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Testing
set(CLEAN_COVERAGE_TARGET coverage-clean)
add_custom_target(${CLEAN_COVERAGE_TARGET}
  COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete
  COMMENT "Removing stale coverage data"
)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/015950a936fc6b7cadca20eb0dbf6f054e6637fb.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(TEST_TARGET ${PROJECT_NAME}_test)
add_executable(${TEST_TARGET}
  test/game_test.cpp
  test/tetromino_test.cpp
)
add_dependencies(${TEST_TARGET} ${CLEAN_COVERAGE_TARGET})
target_include_directories(${TEST_TARGET} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${TEST_TARGET} PRIVATE gamelib GTest::gtest_main)

option(ENABLE_COVERAGE "Enable coverage flags" OFF)
if(ENABLE_COVERAGE)
  target_compile_options(${TEST_TARGET} PRIVATE --coverage -fprofile-arcs -ftest-coverage)
  target_link_options(${TEST_TARGET} PRIVATE --coverage)
endif()

gtest_discover_tests(${TEST_TARGET})


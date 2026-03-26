#include "inputs.hpp"

#include <ncurses.h>
#include <cctype>

auto capture_input(const bool game_started) -> Input {
  if (game_started) {
    constexpr int polling_delay_ms = 16;
    wtimeout(stdscr, polling_delay_ms);
  }

  const int raw_input = getch();
  switch (raw_input) {
    case KEY_LEFT:
      return Input::Left;
    case KEY_RIGHT:
      return Input::Right;
    case ERR:
      return Input::None;
  }

  const char input = std::tolower(raw_input);
  switch(input) {
    case Input::Quit:
    case Input::Start:
    case Input::Left:
    case Input::Right:
    case Input::RotateClockwise:
      return static_cast<Input>(input);
    default:
      return Input::None;
  }
}

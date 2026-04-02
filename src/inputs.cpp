#include "inputs.hpp"

#include <ncurses.h>
#include <cctype>

auto capture_input(const bool game_started) -> Input {
  if (game_started) {
    constexpr int polling_delay_ms = 16;
    wtimeout(stdscr, polling_delay_ms);
  }

  const int raw_input = getch();
  using enum Input;
  switch (raw_input) {
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_DOWN:
      return SoftDrop;
    case ERR:
      return None;
    default:
      break;
  }

  const char input = std::tolower(raw_input);
  switch(input) {
    case Quit:
    case Start:
    case Left:
    case Right:
    case RotateClockwise:
    case RotateAntiClockwise:
    case HardDrop:
      return static_cast<Input>(input);
    default:
      return None;
  }
}

auto input_to_transformation(const Input input) -> std::optional<Transformation> {
  switch (input) {
    case Input::Left:
      return Transformation::Left;
    case Input::Right:
      return Transformation::Right;
    case Input::RotateAntiClockwise:
      return Transformation::RotateAntiClockwise;
    case Input::RotateClockwise:
      return Transformation::RotateClockwise;
    case Input::HardDrop:
      return Transformation::HardDrop;
    case Input::SoftDrop:
      return Transformation::SoftDrop;
    default:
      return std::nullopt;
  }
}

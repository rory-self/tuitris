#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <optional>

enum Input {
  Quit = 'x',
  Start = 's',
  Left = 'a',
  Right = 'd',
  RotateClockwise = 'e',
  RotateAntiClockwise = 'q',
  Drop = ' ',
  None,
};

enum class Transformation {
  Left,
  Right,
  RotateClockwise,
  RotateAntiClockwise,
  Drop,
};

[[nodiscard]] auto capture_input(const bool game_started) -> Input;
[[nodiscard]] auto input_to_transformation(const Input input) -> std::optional<Transformation>;

#endif //INPUTS_HPP

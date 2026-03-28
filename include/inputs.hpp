#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <optional>
#include <cstdint>

enum Input : std::uint8_t {
  Quit = 'x',
  Start = 's',
  Left = 'a',
  Right = 'd',
  RotateClockwise = 'e',
  RotateAntiClockwise = 'q',
  Drop = ' ',
  None,
};

enum class Transformation : std::uint8_t {
  Left,
  Right,
  RotateClockwise,
  RotateAntiClockwise,
  Drop,
};

[[nodiscard]] auto capture_input(bool game_started) -> Input;
[[nodiscard]] auto input_to_transformation(Input input) -> std::optional<Transformation>;

#endif //INPUTS_HPP

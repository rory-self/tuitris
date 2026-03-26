#ifndef INPUTS_HPP
#define INPUTS_HPP

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

[[nodiscard]] auto capture_input(const bool game_started) -> Input;

#endif //INPUTS_HPP

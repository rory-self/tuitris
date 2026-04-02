#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <cstdint>

enum class Transformation : std::uint8_t {
  Left,
  Right,
  RotateClockwise,
  RotateAntiClockwise,
  HardDrop,
  SoftDrop,
};

#endif //TRANSFORMATION_HPP

#ifndef TUI_COLOURS_HPP
#define TUI_COLOURS_HPP

#include "colours.hpp"

#include <ncurses.h>
#include <unordered_map>
#include <array>

class TUIColours {
private:
  static constexpr std::size_t NUM_COLOURS = 7;

  inline static std::unordered_map<Colour, short> _pair_id_by_colour = {};
  static constexpr std::array<std::pair<Colour, short>, NUM_COLOURS> _colour_mappings = {{
    { Colour::Cyan, COLOR_CYAN },
    { Colour::Yellow, COLOR_YELLOW },
    { Colour::Purple, COLOR_MAGENTA },
    { Colour::Blue, COLOR_BLUE },
    { Colour::Green, COLOR_GREEN },
    { Colour::Red, COLOR_RED },
    { Colour::Orange, COLOR_WHITE },
  }};

public:
  [[nodiscard]] static auto colour_to_ncurses_pair(Colour colour) -> chtype;
  static void init();
};

#endif // TUI_COLOURS_HPP

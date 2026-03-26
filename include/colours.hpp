#ifndef COLOURS_HPP
#define COLOURS_HPP

#include <ncurses.h>
#include <unordered_map>

enum class Colour { Cyan, Blue, Orange, Yellow, Green, Purple, Red };

class TUIColours {
private:
  static constexpr std::size_t NUM_COLOURS = 7;

  inline static std::unordered_map<Colour, short> _pair_id_by_colour = {};
  inline static std::pair<Colour, short> _colour_mappings[NUM_COLOURS] = {
    { Colour::Cyan, COLOR_CYAN },
    { Colour::Yellow, COLOR_YELLOW },
    { Colour::Purple, COLOR_MAGENTA },
    { Colour::Blue, COLOR_BLUE },
    { Colour::Green, COLOR_GREEN },
    { Colour::Red, COLOR_RED },
    { Colour::Orange, COLOR_WHITE },
  };

public:
  [[nodiscard]] static auto colour_to_ncurses_pair(const Colour colour) -> chtype;
  static void init();
};

#endif //COLOURS_HPP

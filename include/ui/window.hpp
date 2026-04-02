#include "coordinates.hpp"
#include "colours.hpp"

#include <ncurses.h>
#include <memory>
#include <optional>
#include <functional>

class Window {
private:
  const std::unique_ptr<WINDOW, decltype(&delwin)> _window;

  static std::optional<std::reference_wrapper<Window>> _main_window;

public:
  Window(int height, int width, int y_pos, int x_pos);

  void init_titled(std::string_view title) const;
  void move_print(const Coordinates& pos, std::string_view text) const;
  void draw_border(std::size_t box_bottom_y,
      std::size_t box_top_y, std::size_t box_left_x, std::size_t box_right_x) const;
  void erase() const;
  void refresh() const;
  void print_block(const Coordinates& pos, Colour colour) const;
};

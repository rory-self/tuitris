#include <ncurses.h>
#include <memory>
#include <optional>
#include <functional>

class Window {
private:
  using WindowPtr = std::unique_ptr<WINDOW, decltype(&delwin)>;

  const WindowPtr _window;

  static std::optional<std::reference_wrapper<Window>> _main_window;

  Window(const WINDOW *const window_ptr, );

public:


  [[nodiscard]] static auto init_main_window() -> Window&;
};

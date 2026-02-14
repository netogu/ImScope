#ifndef UI_H
#define UI_H

#include <memory>

namespace Scope {

class Display {

public:
  Display(int pixels_x, int pixels_y);
  ~Display();

  void get_size_px(int &size_x, int &size_y);
  bool window_closing();
  void poll_events();
  void start_frame();
  void render_screen();

private:
  int pixels_x;
  int pixels_y;

  struct Internal;
  std::unique_ptr<Internal> _internal;
};

} // namespace Scope

#endif

#ifndef IMSCOPE_H
#define IMSCOPE_H

#include "imgui.h"
#include "trace.h"
#include "ui.h"
#include "wavegen.h"
#include <string>
#include <vector>

namespace Scope {

static constexpr size_t MAX_POINTS = 2048;
static constexpr int MAX_CHANNELS = 2;

struct Channel {
  Trace<MAX_POINTS> trace;
  ImVec4 color{1.0f, 0.0f, 0.0f, 1.0f};

  Channel(std::string label, ImVec4 c) : trace(std::move(label)), color(c) {}
};

class Scope {
public:
  std::vector<Channel> channels;
  std::vector<WaveformGenerator> generators;

  explicit Scope(Display &d);

  void draw();
  void update();

private:
  Display &_display;
  void draw_plots();
  void draw_channel_library();
  float _time = 0.0f;
};

} // namespace Scope
#endif // !SCOPE_H

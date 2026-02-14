#include "scope.h"
#include "imgui.h"
#include "implot.h"
#include "math.h"
#include <format>
#include <ranges>
#include <vector>

namespace Scope {

template <typename T> static void generate_waveform(Channel &c, T wave_func) {
  for (int i = 0; i < c.trace.size(); ++i) {
    const float dx = 0.001;
    float time = static_cast<float>(i) * dx;
    c.trace.add_sample(time, wave_func(time));
  }
}

static float sinewave_1(float x) {
  return 0.5f + 0.5f * sinf(50 * (x + (float)ImGui::GetTime() / 20));
}

static float sinewave_2(float x) {
  return 0.6f + 0.5f * sinf(30 * (x + (float)ImGui::GetTime() / 18));
}

Scope::Scope(Display &d) : _display(d) {

  channels.reserve(MAX_CHANNELS);
  generators.reserve(MAX_CHANNELS);

  // Channel Color generator
  auto generateColor = [](int index, int total) -> ImVec4 {
    float hue = static_cast<float>(index) / static_cast<float>(total);

    ImVec4 color;
    ImGui::ColorConvertHSVtoRGB(hue, 0.7f, 1.0f, color.x, color.y, color.z);
    color.w = 1.0f;

    return color;
  };

  for (int i = 0; i < MAX_CHANNELS; ++i) {
    channels.emplace_back(std::format("CH{}", i + 1),
                          generateColor(i, MAX_CHANNELS));
    generators.emplace_back();
  }
};

void Scope::update() {
  //
  // Generate Data
  // Add points to the buffers every 0.02 seconds
  static float last_t = 0.0f;
  if (_time == 0 || _time - last_t >= 0.02f) {

    for (auto &&[ch, gen] : std::views::zip(channels, generators)) {
      float offset = ImGui::GetTime() * 2;
      ch.trace.add_sample(_time, gen.generate(_time, offset));
    }
    last_t = _time;
  }
  _time += ImGui::GetIO().DeltaTime;
}

void Scope::draw() {

  // 1. Render the top-most bar
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Clear All Traces")) {
        for (auto &c : channels)
          c.trace.clear();
      }
      if (ImGui::MenuItem("Exit", "Alt+F4")) { /* Handle exit */
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Reset Zoom")) { /* Reset logic */
      }
      ImGui::EndMenu();
    }

    // You can display status info on the right side
    ImGui::TextDisabled("| Time: %.2fs", _time);

    ImGui::EndMainMenuBar();
  }

  // 1. Get the current size of the display
  int px, py;
  _display.get_size_px(px, py);
  float menu_height = ImGui::GetFrameHeight();
  ImGui::SetNextWindowPos(ImVec2(0, menu_height));
  ImGui::SetNextWindowSize(ImVec2((float)px, (float)py - menu_height));

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGui::Begin("MasterLayout", nullptr, flags);

  // 3. Create a Table with 2 columns : "Sidebar" and "Plots"
  // ImGuiTableFlags_Resizable allows the user to drag the border between
  // them
  if (ImGui::BeginTable("MainLayoutTable", 2,
                        ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_BordersInnerV)) {

    // Setup Sidebar Column (Fixed width or initial width)
    ImGui::TableSetupColumn("Sidebar", ImGuiTableColumnFlags_WidthFixed,
                            200.0f);
    // Setup Plot Column (Stretches to fill)
    ImGui::TableSetupColumn("Plots", ImGuiTableColumnFlags_WidthStretch);

    ImGui::TableNextRow();

    // --- LEFT COLUMN: Library ---
    ImGui::TableSetColumnIndex(0);
    this->draw_channel_library();

    // --- RIGHT COLUMN: Oscilloscope ---
    ImGui::TableSetColumnIndex(1);
    this->draw_plots(); // This is your existing draw() logic

    ImGui::EndTable();
  }

  ImGui::End();
}

void Scope::draw_plots() {

  static constexpr float trace_weight = 2.0f;
  static int rows = MAX_CHANNELS;
  constexpr int cols = 1;

  float rratios = 1.0f;
  float cratios = 1.0f;

  static ImPlotSubplotFlags subplot_flags = ImPlotSubplotFlags_LinkAllX;

  if (ImPlot::BeginSubplots("My Subplots", rows, cols, ImVec2(-1, -1),
                            subplot_flags)) {

    for (int i = 0; auto &c : channels) {

      // Plot Channel Data
      if (ImPlot::BeginPlot(std::format("##Plot CH{}", i++ + 2).c_str(),
                            ImVec2(-1, 0))) {
        ImPlot::SetupAxes("x", "y");
        ImPlot::SetupAxisLimits(ImAxis_X1, _time - 10.0f, _time,
                                ImGuiCond_Always);
        ImPlot::SetupLegend(ImPlotLocation_NorthWest, ImPlotLegendFlags_None);
        ImPlot::SetNextLineStyle(c.color, trace_weight);
        ImPlot::PlotLine(c.trace.label.c_str(), c.trace.get_data_x(),
                         c.trace.get_data_y(), c.trace.size(), 0,
                         c.trace.get_data_offset());
        ImPlot::EndPlot();
      }
    }
  }
  ImPlot::EndSubplots();
}

void Scope::draw_channel_library() {
  ImGui::TextDisabled("CHANNELS");
  ImGui::Separator();

  // Begin a child region so the sidebar has its own scrollbar
  if (ImGui::BeginChild("LibraryScrollRegion", ImVec2(0, 0), false)) {
    for (int i = 0; i < channels.size(); ++i) {
      // Visual "Card" for each channel
      ImGui::PushID(i);
      ImColor c = channels[i].color;

      // Draw a small color square next to the name
      ImGui::ColorButton("##color", c, ImGuiColorEditFlags_NoTooltip,
                         ImVec2(12, 12));
      ImGui::SameLine();

      if (ImGui::Selectable(channels[i].trace.label.c_str())) {
        /* logic for selecting */
      }

      // --- DRAG SOURCE ---
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ImGui::SetDragDropPayload("DND_CHANNEL_IDX", &i, sizeof(int));
        ImGui::Text("Dragging %s", channels[i].trace.label.c_str());
        ImGui::EndDragDropSource();
      }

      ImGui::PopID();
    }
  }
  ImGui::EndChild();
}
} // namespace Scope

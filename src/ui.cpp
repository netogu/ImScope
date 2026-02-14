#include "ui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "scope.h"
#include <GLFW/glfw3.h>
#include <cstdio>

namespace Scope {

namespace {
// Callback to handle GLFW errors
void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error: %s", description);
}

GLFWwindow *create_window(int px, int py) {

  // Setup error callback
  glfwSetErrorCallback(glfw_error_callback);

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW");
    return NULL;
  }

  // Setup OpenGL version
#if defined(__APPLE__)
  // GL 3.2 + GLSL 150 (MacOS)
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on MacOS
#else
  // GL 3.0 + GLSL 130 (Windows and Linux)
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  // Create window
  GLFWwindow *window = glfwCreateWindow(px, py, "ImScope", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window");
    glfwTerminate();
    return NULL;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Setup context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  // Setup style
  ImGui::StyleColorsDark();

  // Setup backend
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  return window;
}
} // namespace

struct Display::Internal {
  GLFWwindow *window;
};

Display::Display(int px, int py) : pixels_x(px), pixels_y(py) {
  _internal = std::make_unique<Internal>();
  _internal->window = create_window(pixels_x, pixels_y);
}

Display::~Display() {

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  glfwDestroyWindow(_internal->window);
  glfwTerminate();
}

void Display::get_size_px(int &size_x, int &size_y) {
  glfwGetWindowSize(_internal->window, &size_x, &size_y);
}

bool Display::window_closing() {

  return glfwWindowShouldClose(_internal->window);
}

void Display::poll_events() { glfwPollEvents(); }

void Display::start_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
void Display::render_screen() {

  // Render
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(_internal->window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap buffers
  glfwSwapBuffers(_internal->window);
}

} // namespace Scope

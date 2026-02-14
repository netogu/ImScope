cmake_minimum_required(VERSION 3.23)

project(ImScope)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#------------------------------------------------------------------
# Compiler Settings

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#------------------------------------------------------------------
# Load Libraries

include(FetchContent)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.92.5
)

FetchContent_Declare(
        implot
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG v0.17
)

FetchContent_MakeAvailable(imgui implot)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(imgui_SOURCES
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(implot_SOURCES
  ${implot_SOURCE_DIR}/implot.cpp
  ${implot_SOURCE_DIR}/implot_demo.cpp
  ${implot_SOURCE_DIR}/implot_items.cpp
)

add_library(imgui_static STATIC ${imgui_SOURCES} ${implot_SOURCES})

target_include_directories(imgui_static PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${implot_SOURCE_DIR}
  ${GLFW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(imgui_static PUBLIC
  glfw
  OpenGL::GL
)

target_compile_definitions(imgui_static PUBLIC GL_SILENCE_DEPRECATION)



#-----------------------------------------------------------------------------------------------------------------------
# Create executable

set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(${EXECUTABLE_NAME}
  src/main.cpp
  src/scope.cpp
  src/ui.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  include/
)

# Link against the appropriate libraries
target_link_libraries(${EXECUTABLE_NAME} PRIVATE imgui_static)




cmake_minimum_required(VERSION 3.23)

project(imscope)

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

fetchcontent_declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.92.5
)

fetchcontent_declare(
        implot
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG v0.17
)

fetchcontent_makeavailable(imgui implot)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_SOURCES
  ${IMGUI_SOURCE_DIR}/imgui.cpp
  ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
  ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
  ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
  ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
  ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(IMPLOT_SOURCES
  ${IMPLOT_SOURCE_DIR}/implot.cpp
  ${IMPLOT_SOURCE_DIR}/implot_demo.cpp
  ${IMPLOT_SOURCE_DIR}/implot_items.cpp
)

add_library(imgui_static STATIC ${IMGUI_SOURCES} ${IMPLOT_SOURCES})

target_include_directories(imgui_static PUBLIC
  ${IMGUI_SOURCE_DIR}
  ${IMGUI_SOURCE_DIR}/backends
  ${IMPLOT_SOURCE_DIR}
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




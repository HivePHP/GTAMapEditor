cmake_minimum_required(VERSION 3.16)
project(GTAMapEditor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Добавляем флаг WIN32, чтобы CMake ожидал WinMain вместо main
add_executable(GTAMapEditor WIN32 ${SOURCES} "src/Renderer/Mesh.h" "src/GTA/DFF/DFFParser.h")

target_include_directories(GTAMapEditor PRIVATE src)

# Подключаем OpenGL, GLU и элементы управления Windows
target_link_libraries(GTAMapEditor PRIVATE opengl32 glu32 comctl32) 
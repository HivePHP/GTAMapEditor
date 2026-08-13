cmake_minimum_required(VERSION 3.10)
project(GTAMapEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем все .cpp файлы из наших папок
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} "src/Core/Logger.h" "src/Core/Logger.cpp" "src/GTA/GTAData.h" "src/GTA/GTAData.cpp" "src/GTA/GtaDatParser.h" "src/GTA/GtaDatParser.cpp" "src/GTA/IMG/IMGArchive.h" "src/GTA/IMG/IMGArchive.cpp" "src/GTA/IDE/IDETypes.h" "src/GTA/IDE/IDEParser.h" "src/GTA/IDE/IDEParser.cpp" "src/GTA/IPL/IPLTypes.h" "src/GTA/IPL/IPLParser.h" "src/GTA/IPL/IPLParser.cpp")

# Указываем, где искать заголовочные файлы (.h)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Подключаем стандартные библиотеки Windows и OpenGL
target_link_libraries(${PROJECT_NAME} opengl32 glu32 user32 gdi32)
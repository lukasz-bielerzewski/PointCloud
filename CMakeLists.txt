cmake_minimum_required(VERSION 3.14)

project(PointCloud2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(OpenGL REQUIRED)

add_executable(PointCloud2
  main.cpp
)
target_link_libraries(PointCloud2 Qt${QT_VERSION_MAJOR}::Core
                                  Qt6::Gui
                                  Qt6::OpenGL
                                  Qt6::OpenGLWidgets
                                  OpenGL::GL
                                  OpenGL::GLU)

install(TARGETS PointCloud2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

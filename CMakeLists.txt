cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

pkg_check_modules(OPENGLES2 glesv2 REQUIRED)
pkg_check_modules(EGL egl REQUIRED)

include_directories(Sample_INCLUDES
        ${OPENGLES2_INCLUDE_DIRS}
        ${EGL_INCLUDE_DIRS}
)

link_directories(
    ${OPENGLES2_LIBRARY_DIRS}
    ${EGL_LIBRARY_DIRS}
)

add_definitions("-Wall")

set(Sample_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Sample main.cpp)
target_link_libraries(Sample
    GLESv2
    EGL
    X11
)


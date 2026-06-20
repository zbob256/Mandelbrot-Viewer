cmake_minimum_required(VERSION 3.10)
project(Mandelbrot-Viewer)

find_package(SFML 3 REQUIRED COMPONENTS
    Graphics
    Window
    System
)

add_executable(Mandelbrot-Viewer
    src/main.cpp
    src/renderer.cpp
    src/mandelbrot.cpp
    src/color.cpp
)

target_include_directories(Mandelbrot-Viewer PRIVATE
    include
)


target_link_libraries(Mandelbrot-Viewer PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(MSVC)
    target_compile_options(Mandelbrot-Viewer PRIVATE
        /O2
        /fp:fast
    )
else()
    target_compile_options(Mandelbrot-Viewer PRIVATE
        -O3
        -ffast-math
        -march=native
    )
endif()
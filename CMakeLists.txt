cmake_minimum_required(VERSION 3.20)
project(MonProjet)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

add_executable(mon_app
    ./src/main.cpp
    ./src/window_system.cpp
    ./src/core.cpp
    ./src/system/transform_system.cpp
    ./src/system/render_system.cpp
)

target_compile_options(mon_app PRIVATE
    -g
    -Wall
    -Wextra
    -Wpedantic
    -Werror

    -fsanitize=address,undefined

    -Og
)
target_link_options(mon_app PRIVATE
    -fsanitize=address,undefined
)

target_link_libraries(mon_app PRIVATE
    bgfx
    bimg
    bx
    SDL3::SDL3
)

#############
#   TEST    #
#############

add_executable(mon_test
    "test/utils/sparse_set_test.cpp"
    src/utils/sparse_set.h
    "test/test.cpp"
)

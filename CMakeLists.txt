cmake_minimum_required(VERSION 3.20)
project(MonProjet)

add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

add_compile_options(
    -g
    -Wall
    -Wextra
)

add_executable(mon_app
    src/main.cpp
    src/system/transform_system.cpp
    src/system/render_system.cpp
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
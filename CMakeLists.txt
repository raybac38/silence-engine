cmake_minimum_required(VERSION 3.25)
project(MonProjet)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)

add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory(external/sol2 EXCLUDE_FROM_ALL SYSTEM)


add_executable(mon_app
    ./src/main.cpp
    ./src/manager/window_manager.cpp
    ./src/manager/entity_manager.cpp
    ./src/system/script_system.cpp
    ./src/core.cpp
    ./src/system/transform_system.cpp
    ./src/system/render_system.cpp
    ./src/utils/shader_loader.cpp
    ./src/utils/geometry.cpp
)
# Force the compiler to output C symbole into the dynamic table
set_target_properties(mon_app PROPERTIES ENABLE_EXPORTS TRUE)

target_compile_options(mon_app PRIVATE
    -g
    -Wall
    -Wpedantic
    -Werror

    -fsanitize=address,undefined

    -Og
)
target_link_options(mon_app PRIVATE
    -fsanitize=address,undefined
)
target_include_directories(mon_app SYSTEM PRIVATE
    ${bgfx_SOURCE_DIR}/include
    ${bimg_SOURCE_DIR}/include
    ${bx_SOURCE_DIR}/include
    ${LUAJIT_INCLUDE_DIRS}
)

target_link_directories(mon_app PRIVATE ${LUAJIT_LIBRARY_DIRS})

target_link_libraries(mon_app PRIVATE
    bgfx
    bimg
    bx
    SDL3::SDL3
    sol2::sol2
    ${LUAJIT_LIBRARIES}
)

#############
#   TEST    #
#############


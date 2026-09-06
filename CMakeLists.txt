cmake_minimum_required(VERSION 3.25)
project(MonProjet)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# 2. Utiliser le linker ultra-rapide mold
add_link_options("-fuse-ld=mold")

#----------------------------
#   Submodule dependencies
#----------------------------

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)

add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory(external/sol2 EXCLUDE_FROM_ALL SYSTEM)

# ------------------------------------------------------------------------------
# ASSETS SYMBOLINK LINK
# ------------------------------------------------------------------------------

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets/shaders")

set(ASSET_FOLDERS "obj" "sound" "scripts")

foreach(FOLDER ${ASSET_FOLDERS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/${FOLDER}")
        file(CREATE_LINK 
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/${FOLDER}" 
            "${CMAKE_CURRENT_BINARY_DIR}/assets/${FOLDER}" 
            SYMBOLIC
        )
    endif()
endforeach()

# ------------------------------------------------------------------------------
# LUA API SETUP
# ------------------------------------------------------------------------------


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/api")
    file(CREATE_LINK 
        "${CMAKE_CURRENT_SOURCE_DIR}/api" 
        "${CMAKE_CURRENT_BINARY_DIR}/api" 
        SYMBOLIC
    )
endif()

# ------------------------------------------------------------------------------
# SHADERC & SHANDERS
# ------------------------------------------------------------------------------

set(SHADERC_BIN "${CMAKE_CURRENT_SOURCE_DIR}/external/bgfx/tools/bin/linux/shaderc")

function(compile_bgfx_shader SHADER_NAME SHADER_TYPE)
    set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${SHADER_NAME}.sc")
    set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/assets/shaders/${SHADER_NAME}.bin")
    set(VARYING_DEF "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/varying.def.sc")

    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND ${SHADERC_BIN}
            -f ${INPUT_FILE}
            -o ${OUTPUT_FILE}
            --type ${SHADER_TYPE}
            --platform linux
            -p spirv
            -i "${CMAKE_CURRENT_SOURCE_DIR}/external/bgfx.cmake/bgfx/src"
            --varyingdef ${VARYING_DEF}
        DEPENDS ${INPUT_FILE} ${VARYING_DEF}
        COMMENT "Compilation du shader ${SHADER_NAME}.sc (${SHADER_TYPE})"
    )

    set(COMPILED_SHADERS ${COMPILED_SHADERS} ${OUTPUT_FILE} PARENT_SCOPE)
endfunction()

compile_bgfx_shader(fs_default f)
compile_bgfx_shader(vs_default v)

add_custom_target(compile_shaders DEPENDS ${COMPILED_SHADERS})

# ------------------------------------------------------------------------------
# Main application
# ------------------------------------------------------------------------------

add_executable(mon_app
    ./src/main.cpp
    ./src/manager/window_manager.cpp
    ./src/manager/entity_manager.cpp
    ./src/manager/ressource_manager.cpp
    ./src/system/script_system.cpp
    ./src/core.cpp
    ./src/system/transform_system.cpp
    ./src/system/input_system.cpp
    ./src/system/render_system.cpp
    ./src/utils/shader_loader.cpp
    ./src/utils/geometry.cpp
)

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
    ${CMAKE_CURRENT_BINARY_DIR}
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

add_dependencies(mon_app compile_shaders)

# ------------------------------------------------------------------------------
# Run application
# ------------------------------------------------------------------------------

add_custom_target(run
    COMMAND mon_app
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS mon_app
    USES_TERMINAL
    COMMENT "Running mon_app..."
)
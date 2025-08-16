cmake_minimum_required(VERSION 3.5)
project(Game)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# =========== Dependencies ===========

# bgfx
FetchContent_Declare(
        bgfx
        GIT_REPOSITORY https://github.com/widberg/bgfx.cmake.git
        GIT_TAG master
)

# glfw
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

# glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)

# assimp
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.2
)

# imgui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.1
)

# Set options BEFORE making them available
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set(ASSIMP_INSTALL OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")

set(BGFX_INSTALL ON CACHE BOOL "Install bgfx")
set(BGFX_BUILD_TOOLS ON CACHE BOOL "Build bgfx tools")


FetchContent_MakeAvailable(bgfx glfw glm assimp imgui)

# =========== Shader Compilation ===========

# Define the main source directory for shaders.
set(MAIN_SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

# Find all shader subdirectories by looking for varying.def.sc files.
file(GLOB_RECURSE VARYING_FILES "${MAIN_SHADER_SRC_DIR}/*/varying.def.sc")

# Create a list to hold all the compiled shader binary file paths.
set(SHADER_BINARIES "")

# Loop through each found varying file to compile its associated shaders.
foreach(VARYING_FILE ${VARYING_FILES})
    get_filename_component(SHADER_DIR ${VARYING_FILE} DIRECTORY)

    # Find the vertex and fragment shaders in the same directory.
    file(GLOB VERTEX_SHADER_FILE "${SHADER_DIR}/vs_*.sc")
    file(GLOB FRAGMENT_SHADER_FILE "${SHADER_DIR}/fs_*.sc")

    # MODIFICATION: Set the output directory to be the same as the source directory.
    set(SHADER_OUTPUT_DIR ${SHADER_DIR})

    # We still need the relative path for nice compile messages.
    file(RELATIVE_PATH REL_DIR ${MAIN_SHADER_SRC_DIR} ${SHADER_DIR})

    # --- Compile Vertex Shader ---
    get_filename_component(VS_NAME ${VERTEX_SHADER_FILE} NAME_WE)
    set(VS_OUTPUT_BINARY "${SHADER_OUTPUT_DIR}/${VS_NAME}.bin")
    add_custom_command(
            OUTPUT ${VS_OUTPUT_BINARY}
            COMMAND shaderc # Use the target name directly
            -f ${VERTEX_SHADER_FILE}
            -o ${VS_OUTPUT_BINARY}
            --type vertex
            --varyingdef ${VARYING_FILE}
            --platform windows # Change for other platforms (linux, osx)
            --profile vs_5_0   # Change profile if needed
            -i ${bgfx_SOURCE_DIR}/bgfx/src # CORRECTED: Appended /bgfx/src
            DEPENDS ${VERTEX_SHADER_FILE} ${VARYING_FILE}
            COMMENT "Compiling Vertex Shader: ${REL_DIR}/${VS_NAME}.sc"
    )
    # Add the output file path to our list.
    list(APPEND SHADER_BINARIES ${VS_OUTPUT_BINARY})

    # --- Compile Fragment Shader ---
    get_filename_component(FS_NAME ${FRAGMENT_SHADER_FILE} NAME_WE)
    set(FS_OUTPUT_BINARY "${SHADER_OUTPUT_DIR}/${FS_NAME}.bin")
    add_custom_command(
            OUTPUT ${FS_OUTPUT_BINARY}
            COMMAND shaderc # Use the target name directly
            -f ${FRAGMENT_SHADER_FILE}
            -o ${FS_OUTPUT_BINARY}
            --type fragment
            --varyingdef ${VARYING_FILE}
            --platform windows # Change for other platforms (linux, osx)
            --profile ps_5_0   # Change profile if needed
            -i ${bgfx_SOURCE_DIR}/bgfx/src # CORRECTED: Appended /bgfx/src
            DEPENDS ${FRAGMENT_SHADER_FILE} ${VARYING_FILE}
            COMMENT "Compiling Fragment Shader: ${REL_DIR}/${FS_NAME}.sc"
    )
    # Add the output file path to our list.
    list(APPEND SHADER_BINARIES ${FS_OUTPUT_BINARY})
endforeach()

# A custom target to group all shader compilation steps.
# This target depends on the list of files we just created.
add_custom_target(Shaders ALL DEPENDS ${SHADER_BINARIES})


# =========== Build Libraries ===========

# imgui
add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        #Backends
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# glad
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/3rdparty/glad")
add_library(glad STATIC
        ${GLAD_DIR}/src/glad.c
)

# =========== Source Files ===========

file(GLOB_RECURSE SRC_FILES
        ${CMAKE_SOURCE_DIR}/engine/*.cpp
        ${CMAKE_SOURCE_DIR}/game/*.cpp
)

add_executable(Game main.cpp ${SRC_FILES})

# Make the Game executable depend on the shaders being compiled first.
add_dependencies(Game Shaders)


# =========== Include Directories ===========

target_include_directories(Game PRIVATE
        ${CMAKE_SOURCE_DIR}/engine
        ${CMAKE_SOURCE_DIR}/game
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${glfw_SOURCE_DIR}/include
)

target_include_directories(glad PUBLIC
        ${GLAD_DIR}/include
)


# =========== Linking ===========
target_link_libraries(Game PRIVATE bgfx glfw glm assimp imgui glad)

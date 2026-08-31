cmake_minimum_required(VERSION 3.20)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
# Add this line to force CMake 3.30+ to accept Raylib's older CMakeLists.txt
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
project(OrbitTrack CXX)

# Enforce modern C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable FetchContent module
include(FetchContent)

# 1. Fetch Raylib (Has a native CMakeLists.txt)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master # Changed from 5.0 to master
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# 2. Fetch ImGui (Raw source files, no native CMakeLists.txt)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking # Use 'docking' or 'master'
)
FetchContent_Populate(imgui)

# 3. Fetch rlImGui (Raylib-ImGui bindings)
FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
)
FetchContent_Populate(rlimgui)

# sgp4
FetchContent_Declare(
    sgp4
    GIT_REPOSITORY https://github.com/dnwrnr/sgp4.git
    GIT_TAG master
)
FetchContent_MakeAvailable(sgp4)

# curl
set(CURL_USE_SCHANNEL ON)   # Windows native TLS, skip OpenSSL dependency
set(BUILD_CURL_EXE OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_10_1
)
FetchContent_MakeAvailable(curl)

# Define the executable and compile the downloaded source files directly
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/satellite/satellite.cpp
    src/parse_data.cpp
    src/render/camera.cpp
    src/render/simulator/simulator.cpp
    src/second_thread/second_thread.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

# Link Raylib (ImGui and rlImGui are compiled directly into the executable above)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    raylib
    sgp4
    libcurl
)

# Point the compiler to the downloaded header files
target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    ${imgui_SOURCE_DIR}
    ${rlimgui_SOURCE_DIR}
)

# Post-build step: Copy the assets/ folder to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_TARGET_FILE_DIR}/assets
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
cmake_minimum_required(VERSION 3.10)
project(SnowInvaders)

# Find SDL2 and its related libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_NET REQUIRED SDL2_net)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_NET_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src  # Include the src directory for header files
)

# Link directories for the libraries
link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
    ${SDL2_NET_LIBRARY_DIRS}
)

# Add source files from the src directory
file(GLOB SOURCE_FILES "src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link SDL2 and related libraries
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
)

# Ensure proper C++ standard (if you're using C++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.16)

# ==============================================================================
# Project Information
# Project Title: Image Colour Converter
# Subtitle: An Object-Oriented Image Colour Conversion Application
# Team: Team 13
# Team Members:
#   1. PARIHAR SHARDUL RAJENDRA - SEITB243
#   2. WANKHADE ISHAN VIJAY     - SEITB266
# Department: Department of Information Technology
# Institute: N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
# Academic Review: First Review - Mini Project (OOP)
# ==============================================================================

project(image_colour_converter
    VERSION 1.0.0
    DESCRIPTION "An Object-Oriented Image Colour Conversion Application using C++17 and OpenCV"
    LANGUAGES CXX
)

# Enforce C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for language servers (clangd, VS Code, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Dependency Discovery: OpenCV 4.x
# ------------------------------------------------------------------------------
# In case OpenCV is installed in a custom location, users can specify:
#   cmake -B build -DOpenCV_DIR=C:/path/to/opencv/build
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui)

message(STATUS "--------------------------------------------------------")
message(STATUS "Image Colour Converter Build Configuration:")
message(STATUS "  C++ Standard:      C++${CMAKE_CXX_STANDARD}")
message(STATUS "  C++ Compiler:      ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  OpenCV Version:    ${OpenCV_VERSION}")
message(STATUS "  OpenCV Include:    ${OpenCV_INCLUDE_DIRS}")
message(STATUS "  OpenCV Libraries:  ${OpenCV_LIBS}")
message(STATUS "--------------------------------------------------------")

# ------------------------------------------------------------------------------
# Include Directories
# ------------------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# ------------------------------------------------------------------------------
# Source Files Definition
# ------------------------------------------------------------------------------
set(HEADER_FILES
    include/ImageManager.h
    include/ColourProcessor.h
    include/OutputManager.h
    include/ImageConverterApp.h
)

set(SOURCE_FILES
    src/ImageManager.cpp
    src/ColourProcessor.cpp
    src/OutputManager.cpp
    src/ImageConverterApp.cpp
    src/main.cpp
)

# ------------------------------------------------------------------------------
# Executable Target
# ------------------------------------------------------------------------------
add_executable(image_colour_converter
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(image_colour_converter
    PRIVATE
    ${OpenCV_LIBS}
)

# Compiler Warnings and Strictness
if(MSVC)
    target_compile_options(image_colour_converter PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(image_colour_converter PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ------------------------------------------------------------------------------
# Asset and Output Directory Deployment
# ------------------------------------------------------------------------------
# Ensure assets and sample directories exist in build directory for smooth execution
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_output)

# Copy assets to build tree if present
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
endif()

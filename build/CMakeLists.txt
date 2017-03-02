# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(opencv_example_project)

# Output for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/)

# Define common cpps
set (
    COMMON_CPPS
    ../src/timer/timer.cpp
    ../src/config/config.cpp
    ../src/io/Capture.cpp
    ../src/gui/InitialScreen.cpp
    ../src/gui/PlayingScreen.cpp
    ../src/gui/InstructionsText.cpp
    ../src/game/Game.cpp
    ../src/gui/Screen.cpp
    ../src/tracking/ColorSampler.cpp
    ../src/tracking/Tracker.cpp
    iht.o
)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_BINARY_DIR}/modules")
# set(OpenCV_DIR "${CMAKE_BINARY_DIR}/modules")
find_package(OpenCV REQUIRED)

add_definitions (
    --std=c++11
    -g
    -O3
)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
  # Add OpenCV headers location to your include paths
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# assemble hist.asm
add_custom_command (
    OUTPUT hist.o
    COMMAND nasm -f elf64 -g -F dwarf ../asm/hist.asm -o hist.o
    DEPENDS ../asm/hist.asm
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# assemble backproject.asm
add_custom_command (
    OUTPUT backproject.o
    COMMAND nasm -f elf64 -g -F dwarf ../asm/backproject.asm -o backproject.o
    DEPENDS ../asm/backproject.asm
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# assemble meanshift.asm
add_custom_command (
    OUTPUT meanshift.o
    COMMAND nasm -f elf64 -g -F dwarf ../asm/meanshift.asm -o meanshift.o
    DEPENDS ../asm/meanshift.asm
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# compile iht separately to control optimization flags
add_custom_command (
    OUTPUT iht.o
    COMMAND g++ -O3 -g -c -m64 ../src/in-house-tracking/iht.cpp -o iht.o --std=c++11
    DEPENDS ../src/in-house-tracking/iht.cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Declare the executable target built from your sources
add_executable (
    main
    hist.o
    backproject.o
    meanshift.o
    ../src/game/main.cpp
    ${COMMON_CPPS}
)

add_executable (
    exp
    hist.o
    backproject.o
    meanshift.o
    ../exp/times.cpp
    ${COMMON_CPPS}
)

# Link your application with OpenCV libraries
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(exp ${OpenCV_LIBS})

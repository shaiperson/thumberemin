# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(opencv_example_project)

# Output for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/)

# Define common cpps
set (
    COMMON_CPPS
    ../src/config.cpp
    ../src/Capture.cpp
    ../src/InitialScreen.cpp
    ../src/PlayingScreen.cpp
    ../src/InstructionsText.cpp
    ../src/Theremin.cpp
    ../src/Screen.cpp
    ../src/ColorSampler.cpp
    ../src/Tracker.cpp
    ../src/SilentSoundGenerator.cpp
    ../src/RangeSoundGenerator.cpp
    ../src/ContinuousSineWave.cpp
)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_BINARY_DIR}/modules")
# set(OpenCV_DIR "${CMAKE_BINARY_DIR}/modules")
find_package(OpenCV REQUIRED)

#juancho
add_definitions(--std=c++11)

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

# Declare the executable target built from your sources
add_executable (
    prueba
    ../src/main.cpp
    ${COMMON_CPPS}
)

add_executable (
    tests
    ../test/catch.cpp
    ../test/ContinuousSineWaveTest.cpp
    ${COMMON_CPPS}
)

add_executable (
    exp
    ../exp/cswExp.cpp
    ../exp/exp.cpp
    ${COMMON_CPPS}
)

# Link your application with OpenCV libraries
target_link_libraries(prueba ${OpenCV_LIBS} portaudio)
target_link_libraries(tests ${OpenCV_LIBS} portaudio)
target_link_libraries(exp ${OpenCV_LIBS} portaudio)
# target_link_libraries(prueba portaudio)

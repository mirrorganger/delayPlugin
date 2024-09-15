cmake_minimum_required(VERSION 3.15)
project(delayPlugin VERSION 0.1)

include(FetchContent)

FetchContent_Declare(
    juce 
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 46c2a95 # v8.0.1
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(juce)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
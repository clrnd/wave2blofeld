cmake_minimum_required(VERSION 3.0)
project(wave2blofeld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb3 -g")

link_directories(deps/midifile/lib)

include_directories(deps/AudioFile)
include_directories(deps/midifile/include)

file(GLOB SOURCES "src/*.cpp")
file(GLOB SOURCES_AUDIOFILE "deps/AudioFile/*.cpp")

add_executable(wave2blofeld ${SOURCES} ${SOURCES_AUDIOFILE})
target_link_libraries(wave2blofeld midifile)

cmake_minimum_required(VERSION 3.0)
project(wave2blofeld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb3 -g")

link_directories(deps/midifile-master/lib)

include_directories(deps/midifile-master/include)
include_directories(deps/audiofile-master)
include_directories(deps/tclap-1.2.1/include)

file(GLOB SOURCES "src/*.cpp" "deps/audiofile-master/*.cpp")

add_executable(wave2blofeld ${SOURCES})
target_link_libraries(wave2blofeld midifile)

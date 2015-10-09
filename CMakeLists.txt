cmake_minimum_required(VERSION 3.3)
project(IrcSockets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/WindowsSockets.h include/LinuxSockets.h
    src/WindowsSockets.cpp src/LinuxSockets.cpp
    main.cpp src/IrcBot.cpp include/IrcBot.h)

add_executable(IrcSockets ${SOURCE_FILES})
target_link_libraries(IrcSockets ${CMAKE_SOURCE_DIR}/libws2_32.a)
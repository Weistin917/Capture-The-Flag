cmake_minimum_required(VERSION 3.16)
project(CaptureTheFlag CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

add_executable(ctf
    main.cpp
    utils/args.cpp
    coms/discovery.cpp
)

target_include_directories(ctf PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ctf PRIVATE nlohmann_json::nlohmann_json)

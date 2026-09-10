cmake_minimum_required(VERSION 3.10)
project("Rock-stay")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio Network)

file(GLOB SOURCES "*.cpp" "*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
)

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Executable name: ${PROJECT_NAME}")

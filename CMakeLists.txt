cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)

project(procmanager)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
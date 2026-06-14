cmake_minimum_required(VERSION 3.14)
project(ChatApp)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "BUILDING DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    message(STATUS "BUILDING RELEASE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

find_package(PostgreSQL REQUIRED)

set(CROW_BUILD_EXAMPLES OFF)
set(CROW_BUILD_TESTS OFF)
add_subdirectory(crow)
add_executable(server main.cpp
)

target_link_libraries(server PUBLIC PostgreSQL::PostgreSQL pq Crow::Crow)
target_include_directories(server PUBLIC ${PostgreSQL_INCLUDE_DIRS})
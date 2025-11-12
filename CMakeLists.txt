cmake_minimum_required(VERSION 3.14)
project(ChatApp)

set(CMAKE_CXX_STANDARD 20)

find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-postgresql REQUIRED)
find_package(PostgreSQL REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "BUILDING DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    message(STATUS "BUILDING RELEASE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(server main.cpp
        controller.cpp
        controller.hpp
        userService.cpp
        userService.hpp
        DbDelegate.cpp
        DbDelegate.hpp
        dto.cpp
        dto.hpp
        ObjectMapper.hpp
)

target_link_libraries(server PUBLIC oatpp::oatpp oatpp::oatpp-postgresql pq)
target_include_directories(server PUBLIC ${PostgreSQL_INCLUDE_DIRS})
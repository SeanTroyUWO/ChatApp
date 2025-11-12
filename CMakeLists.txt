cmake_minimum_required(VERSION 3.14)
project(ChatApp)

set(CMAKE_CXX_STANDARD 20)

find_package(oatpp 1.4.0 REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(server main.cpp
        controller.cpp
        controller.hpp
        userService.cpp
        userService.hpp
        DbDelegate.cpp
        DbDelegate.hpp
        dto.cpp
        dto.hpp)

target_link_libraries(server PUBLIC oatpp::oatpp pq)
target_include_directories(server PUBLIC ${PostgreSQL_INCLUDE_DIRS})

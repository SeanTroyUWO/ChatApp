cmake_minimum_required(VERSION 3.14)
project(ChatApp)

set(CMAKE_CXX_STANDARD 20)

find_package(oatpp 1.4.0 REQUIRED)

add_executable(server main.cpp)

target_link_libraries(server PUBLIC oatpp::oatpp)

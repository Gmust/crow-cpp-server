cmake_minimum_required(VERSION 3.10)
project(webserver)

find_package(Boost REQUIRED COMPONENTS system date_time)

add_executable(webserver main.cpp
        todo_controllers.cpp
        todo_functions.cpp)

target_link_libraries(webserver PRIVATE ${Boost_LIBRARIES})
target_link_libraries(webserver PRIVATE ws2_32 mswsock)

target_include_directories(webserver PRIVATE ${Boost_INCLUDE_DIRS})

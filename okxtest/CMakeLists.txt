cmake_minimum_required(VERSION 3.26)
project(okxtest)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(websocketpp REQUIRED)
include_directories(${websocketpp_INCLUDE_DIRS})

include_directories(src)
include_directories(include)

add_executable(okxtest
        include/OKXConnector.h
        src/OKXConnector.cpp
        main.cpp
        include/ComplexCalculator.h
        src/ComplexCalculator.cpp
)

target_link_libraries(okxtest
        ${Boost_LIBRARIES}
        websocketpp::websocketpp
)

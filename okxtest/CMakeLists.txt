cmake_minimum_required(VERSION 3.10)
project(okxtest)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(websocketpp REQUIRED)
include_directories(${websocketpp_INCLUDE_DIRS})
find_package(OpenSSL REQUIRED)
#find_package(nlohmann_json REQUIRED)

add_executable(okxtest
        src/OKXConnector.cpp
        main.cpp
        src/ComplexCalculator.cpp
)

target_include_directories(okxtest PUBLIC
        include
)

target_link_libraries(okxtest
        ${Boost_LIBRARIES}
        websocketpp::websocketpp
        OpenSSL::SSL OpenSSL::Crypto
#        nlohmann_json::nlohmann_json
)

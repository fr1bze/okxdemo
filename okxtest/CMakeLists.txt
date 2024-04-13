cmake_minimum_required(VERSION 3.20)
project(okxtest)
set(CMAKE_CXX_STANDARD 20)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ERROR_CODE_HEADER_ONLY -pthread")

find_package(Boost REQUIRED)
find_package(websocketpp REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${websocketpp_INCLUDE_DIRS}
        ${OpenSSL_INCLUDE_DIRS}
        ${nlohmann_json_INCLUDE_DIRS}
        include
        src
)

add_executable(okxtest
        src/OKXConnector.cpp
        okxtest_client.cpp
        src/ComplexCalculator.cpp
)

# Link libraries
target_link_libraries(okxtest
        ${Boost_LIBRARIES}
        websocketpp::websocketpp
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
)

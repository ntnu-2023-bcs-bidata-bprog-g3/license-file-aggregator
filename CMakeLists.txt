cmake_minimum_required(VERSION 3.0)
project(license_consumer LANGUAGES CXX)

find_package(Boost 1.74.0 REQUIRED)
find_package(Crow)

add_executable(license_consumer main.cpp)
target_link_libraries(license_consumer PUBLIC Crow::Crow)
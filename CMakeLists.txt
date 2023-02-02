cmake_minimum_required(VERSION 3.13)
project(license_consumer)

add_executable(app "")

find_package(Boost 1.74.0 REQUIRED)
find_package(Crow REQUIRED)
target_link_libraries(app PUBLIC Crow::Crow)

add_subdirectory(src)
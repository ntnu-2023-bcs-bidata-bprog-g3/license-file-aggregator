cmake_minimum_required(VERSION 3.1)

set(project_name lfa) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/error/error.hpp
        src/error/error.cpp
        src/controller/Controller.cpp
        src/controller/Controller.hpp
        src/dto/DTOs.hpp
        src/file/fileHandler.cpp
        src/file/fileHandler.hpp
        src/ssl/certificates.cpp
        src/ssl/certificates.hpp
)


## link libs

find_package(oatpp 1.3.0 REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(oatpp-openssl)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test    
        PUBLIC oatpp-openssl
        PUBLIC ${Boost_LIBRARIES}
)

target_include_directories(${project_name}-lib PUBLIC src)
target_include_directories(${project_name}-lib PUBLIC ${Boost_INCLUDE_DIRS})

## add executables

add_executable(${project_name}-exe
        src/App.cpp
        test/app/ApiTestClient.hpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/app/TestComponent.hpp
        test/app/ApiTestClient.hpp
        test/ControllerTest.cpp
        test/ControllerTest.hpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)

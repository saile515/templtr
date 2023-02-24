cmake_minimum_required(VERSION 3.15)

if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")
endif()

include(cmake/Vcpkg.cmake)

project(templtr 
        VERSION 0.1.0
        LANGUAGES CXX)

find_package(fmt CONFIG REQUIRED)

add_executable(templtr src/main.cpp)

target_link_libraries(templtr PRIVATE fmt::fmt)

add_custom_target(run-ex1
COMMAND templtr 
DEPENDS templtr 
WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
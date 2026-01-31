cmake_minimum_required(VERSION 3.14)
project(PathPlanningLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library
add_library(path_planning STATIC
    src/graph.cpp
    src/uninformed_search.cpp
    src/informed_search.cpp
    src/dynamic_search.cpp
    src/sampling_based.cpp
)

target_include_directories(path_planning PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Test executable
add_executable(path_planning_test tests/test.cpp)
target_link_libraries(path_planning_test path_planning)

# Example executable
add_executable(simple_example examples/simple_example.cpp)
target_link_libraries(simple_example path_planning)

# Comprehensive test executable
add_executable(comprehensive_test tests/comprehensive_test.cpp)
target_link_libraries(comprehensive_test path_planning)

# Usage demo executable
add_executable(usage_demo USAGE_DEMO.cpp)
target_link_libraries(usage_demo path_planning)

# Install
install(TARGETS path_planning
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

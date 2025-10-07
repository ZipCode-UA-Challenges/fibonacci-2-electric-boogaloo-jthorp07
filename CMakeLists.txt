cmake_minimum_required(VERSION 3.30.0)
project(Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dirs
add_subdirectory(include)
add_subdirectory(src)

# Executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE src)

# Test Executable
add_executable(test "tests/test.cpp")
target_link_libraries(test include)
target_link_libraries(test src)

cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_TOOLCHAIN_FILE "D:\\Library\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
project(hello_opengl)

find_package(glad)
find_package(glm)
find_package(glfw3)
find_package(GTest CONFIG REQUIRED)

add_executable(
    ${PROJECT_NAME}_test
    draw_triangle_test.cpp
    chaos.cpp
    unit_test.cpp
)
target_link_libraries(${PROJECT_NAME}_test glad::glad glfw glm::glm GTest::gtest)
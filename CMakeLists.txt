cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_TOOLCHAIN_FILE "D:\\Library\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
set(CMAKE_CXX_STANDARD 20)
project(triangles_shower)

find_package(glad)
find_package(glm)
find_package(glfw3)
find_package(GTest CONFIG REQUIRED)

add_executable(
    ${PROJECT_NAME}_test
    draw_triangle_test.cpp
    chaos.cpp
    chaos_shader.cpp
    chaos_camera.cpp
    chaos_shower.cpp
    mvp_test.cpp
    # camera_move_test.cpp
    phong_lighting_test.cpp
    unit_test.cpp
)
target_link_libraries(${PROJECT_NAME}_test glad::glad glfw glm::glm GTest::gtest)

add_library(
    ${PROJECT_NAME}
    opengl_chaos.cpp
    chaos_shower.cpp
    chaos_shader.cpp
    chaos_camera.cpp
    chaos_shower.cpp
    triangles_shower.cpp
)

target_link_libraries(${PROJECT_NAME} glad::glad glfw glm::glm)

# 引入要用到的 CMake 模块
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# 基本安装及 Targets 文件的生成
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Targets 文件的安装
install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Config 文件的导出（与安装？）
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# ConfigVersion 文件的导出
write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)

# Config 和 ConfigVersion 文件的安装
install(FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES "${PROJECT_SOURCE_DIR}/opengl_chaos.h" DESTINATION include)
install(FILES "${PROJECT_SOURCE_DIR}/chaos_shower.h" DESTINATION include)
install(FILES "${PROJECT_SOURCE_DIR}/chaos_shader.h" DESTINATION include)
install(FILES "${PROJECT_SOURCE_DIR}/chaos_camera.h" DESTINATION include)
install(FILES "${PROJECT_SOURCE_DIR}/triangles_shower.h" DESTINATION include)


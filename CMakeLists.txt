cmake_minimum_required(VERSION 3.5)

## vcpkg
SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

## project
project (
	ImGui-GLFW-OpenGL3
	DESCRIPTION "ImGui Sample with GLFW + OpenGL3"
	LANGUAGES CXX)

## Find dependencies
find_package(glfw3 REQUIRED)

# Use  cmake -S . -B build/ -DOpenGL_GL_PREFERENCE=GLVND
# to skip legacy OpenGL library
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(imgui REQUIRED)

## Create main executable
add_executable(ImGui-GLFW-OpenGL3)

target_include_directories(
	ImGui-GLFW-OpenGL3
  	PRIVATE 
	${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/implot
        ${CMAKE_SOURCE_DIR}/imgui-knobs
        ${CMAKE_SOURCE_DIR}/imspinner
)

IF (WIN32)
    target_link_libraries(ImGui-GLFW-OpenGL3 PRIVATE glfw opengl32.lib imgui::imgui)
ELSE()
	target_link_libraries(ImGui-GLFW-OpenGL3 PRIVATE glfw OpenGL imgui::imgui)
ENDIF()


target_sources(
	ImGui-GLFW-OpenGL3
	PRIVATE
        ${CMAKE_SOURCE_DIR}/main.cpp

        ${CMAKE_SOURCE_DIR}/implot/implot.cpp
        ${CMAKE_SOURCE_DIR}/implot/implot_items.cpp
        ${CMAKE_SOURCE_DIR}/implot/implot_demo.cpp

        ${CMAKE_SOURCE_DIR}/imgui-knobs/imgui-knobs.cpp
)


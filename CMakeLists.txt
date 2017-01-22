cmake_minimum_required(VERSION 3.3)
project(gl_utilities LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw REQUIRED)

include_directories(include ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLFW_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 14)

add_library(gl_utilities SHARED
	src/gl_utilities/glew/error.cpp
	src/gl_utilities/glew/init.cpp
	src/gl_utilities/glfw/init.cpp
	src/gl_utilities/glfw/window.cpp
	src/gl_utilities/opengl/active_texture.cpp
	src/gl_utilities/opengl/basic_error.cpp
	src/gl_utilities/opengl/buffer.cpp
	src/gl_utilities/opengl/clear_color.cpp
	src/gl_utilities/opengl/enable.cpp
	src/gl_utilities/opengl/error.cpp
	src/gl_utilities/opengl/frame_buffer.cpp
	src/gl_utilities/opengl/polygon_mode.cpp
	src/gl_utilities/opengl/primitive_restart_index.cpp
	src/gl_utilities/opengl/program.cpp
	src/gl_utilities/opengl/render_buffer.cpp
	src/gl_utilities/opengl/shader.cpp
	src/gl_utilities/opengl/texture.cpp
	src/gl_utilities/opengl/vertex_array.cpp
	src/gl_utilities/opengl/viewport.cpp
)
target_link_libraries(gl_utilities ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARY})

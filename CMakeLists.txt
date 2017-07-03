cmake_minimum_required(VERSION 3.6)
project(pac_man)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Shader.cpp src/Shader.h src/Level.cpp src/Level.h src/Player.cpp src/Player.h src/Enemy.cpp src/Enemy.h src/Gem.cpp src/Gem.h src/Mesh.cpp src/Mesh.h src/Map.cpp src/Map.h src/Scene.cpp src/Scene.h src/PowerUp.cpp src/PowerUp.h)
add_executable(pac_man ${SOURCE_FILES})

#SOIL
include_directories(${PROJECT_SOURCE_DIR}/dependecies/soil-master/inc)
add_subdirectory(${PROJECT_SOURCE_DIR}/dependecies/soil-master)

#GLM
include_directories(${PROJECT_SOURCE_DIR}/dependecies/glm)

#OpenGL
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    link_libraries(${OPENGL_LIBRARIES})
endif()

#GLEW
include_directories(${PROJECT_SOURCE_DIR}/dependecies/glew-1.9.0/include/GL)
set (GLEW_LIB ${PROJECT_SOURCE_DIR}/dependecies/glew-1.9.0/bin/glew32.dll)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependecies/glfw-3.2.1)

target_link_libraries(pac_man soil ${GLEW_LIB} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} glfw)
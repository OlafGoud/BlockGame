cmake_minimum_required(VERSION 3.15)
project(BlockGame VERSION 1.0 LANGUAGES CXX)

# -------------------------
# C++ standard
# -------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# MSVC runtime library
# -------------------------
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -------------------------
# Dependencies
# -------------------------
find_package(OpenGL REQUIRED)

include(FetchContent)

# ----- GLAD -----
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# ----- GLFW -----
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/minizip")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include/minizip")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_USE_MSVC_RUNTIME_LIBRARY TRUE CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# ----- FreeType -----
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-1
)
FetchContent_MakeAvailable(freetype)

# ----- Assimp -----
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(assimp)

# ----- GLM -----
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# ----- STB -----
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# -------------------------
# Target
# -------------------------
add_executable(${PROJECT_NAME} 
  src/main.cpp 
  src/input.cpp
  src/shader.cpp
  src/camera.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
  PROJECT_BINARY_DIR="${CMAKE_BINARY_DIR}"
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${PROJECT_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
  ${stb_SOURCE_DIR}  
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  OpenGL::GL
  glad
  glfw
  freetype
  assimp
)

# -------------------------
# Resources
# -------------------------
# file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# -------------------------
# Optional: suppress LNK4098 warning
# -------------------------
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/NODEFAULTLIB:MSVCRT")
endif()



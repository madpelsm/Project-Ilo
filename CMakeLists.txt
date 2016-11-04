cmake_minimum_required(VERSION 2.6)
project(Ilo)
cmake_policy(SET CMP0054 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules/)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-variable")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()


if (WIN32)
    # Please don't define min() and max() macros, Windows
    add_definitions(-DNOMINMAX)
endif()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)

include_directories(glad/include/)
include_directories(${SDL2_INCLUDE_DIR}/SDL2)
include_directories(${GLM_INCLUDE_DIR})

file(GLOB ILO_SRC_FILES "./*.cpp")
file(GLOB ILO_HDR_FILES "./*.h")

set(ILO_SRC_FILES ${ILO_SRC_FILES} tiny_obj_loader.cc)
set(GLAD_SRC_FILES glad/src/glad.c)

add_executable(ilo ${ILO_SRC_FILES} ${GLAD_SRC_FILES} ${ILO_HDR_FILES})

target_link_libraries(ilo ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${CMAKE_DL_LIBS})
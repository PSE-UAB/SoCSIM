cmake_minimum_required(VERSION 3.1)

# set the project name
project(SoCSIM)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra -pedantic -fPIC -pthread -O3)

include_directories(./GUI/inc/)
file(GLOB SRC_GUI "GUI/*.cpp")
file (GLOB SRC_FREERTOS "../FreeRTOS-Kernel/*.c" "../freertos-addons/Linux/portable/GCC/Linux/*.c" "../FreeRTOS-Kernel/portable/MemMang/heap_3.c")
file(GLOB SRC_SIM "SIM/*.cpp" "SIM/*.c")

include_directories(.)
include_directories(../FreeRTOS-Kernel/include)
include_directories(../freertos-addons/Linux/portable/GCC/Linux)

find_package(SDL2 REQUIRED)
include_directories(SoCSIM ${SDL2_INCLUDE_DIRS})

if(${CMAKE_VERSION} VERSION_LESS "3.11.0")
    set(OpenGL_GL_PREFERENCE GLVND)
else()
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

add_executable(SoCSIM main.c ${SRC_GUI} ${SRC_FREERTOS} ${SRC_SIM})

target_link_libraries(SoCSIM ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} Threads::Threads)

target_compile_definitions(SoCSIM PRIVATE IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_compile_definitions(SoCSIM PRIVATE _REENTRANT)

option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/SoCSIM.doxygen)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/doc)
    message("Doxygen build started")
    add_custom_target( doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
            WORKING_DIRECTORY ${DOXYGEN_OUT}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)



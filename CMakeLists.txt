cmake_minimum_required(VERSION 3.1)

# set the project name
project(Prac)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -pedantic -fPIC -pthread -O3)

include_directories(./GUI/inc/)
file(GLOB SOURCES "GUI/*.cpp" "SIM/*.cpp" "../../../FreeRTOS/Source/*.c" "../../Linux/portable/GCC/Linux/*.c" "../../../FreeRTOS/Source/portable/MemMang/heap_3.c")

include_directories(.)
include_directories(../../../FreeRTOS/Source/include)
include_directories(../../Linux/portable/GCC/Linux)

find_package(SDL2 REQUIRED)
include_directories(Prac ${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)


# add the executable
add_executable(Prac main.c ${SOURCES})

target_link_libraries(Prac ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} Threads::Threads)

target_compile_definitions(Prac PRIVATE IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_compile_definitions(Prac PRIVATE _REENTRANT)




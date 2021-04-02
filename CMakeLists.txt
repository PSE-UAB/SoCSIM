cmake_minimum_required(VERSION 3.1)

# set the project name
project(SoCSIM)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra -pedantic -fPIC -pthread -O3)

include_directories(./GUI/inc/)
file(GLOB SRC_GUI "GUI/*.cpp")
file (GLOB SRC_FREERTOS "../FreeRTOS-Kernel/*.c" "../freertos-addons/Linux/portable/GCC/Linux/*.c" "../FreeRTOS-Kernel/portable/MemMang/heap_3.c")
file(GLOB SRC_SIM "SIM/*.cpp")

include_directories(.)
include_directories(../FreeRTOS-Kernel/include)
include_directories(../freertos-addons/Linux/portable/GCC/Linux)

find_package(SDL2 REQUIRED)
include_directories(SoCSIM ${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

add_executable(SoCSIM main.c ${SRC_GUI} ${SRC_FREERTOS} ${SRC_SIM})

target_link_libraries(SoCSIM ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} Threads::Threads)

target_compile_definitions(SoCSIM PRIVATE IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_compile_definitions(SoCSIM PRIVATE _REENTRANT)




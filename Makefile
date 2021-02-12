#############################################################################
#
#  Copyright (c) 2017, Michael Becker (michael.f.becker@gmail.com)
#
#  This file is part of the FreeRTOS Add-ons project.
#
#  Source Code:
#  https://github.com/michaelbecker/freertos-addons
#
#  Project Page:
#  http://michaelbecker.github.io/freertos-addons/
#
#  On-line Documentation:
#  http://michaelbecker.github.io/freertos-addons/docs/html/index.html
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so,subject to the following conditions:
#
#  + The above copyright notice and this permission notice shall be included
#    in all copies or substantial portions of the Software.
#  + Credit is appreciated, but not required, if you find this project useful
#    enough to include in your application, product, device, etc.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#
#############################################################################


FREERTOS_SRC_DIR=../../../FreeRTOS/Source
FREERTOS_INC_DIR=../../../FreeRTOS/Source/include
PORTABLE_SRC_DIR=../../Linux/portable/GCC/Linux
PORTABLE_SRC_MEM_MANG_DIR=../../../FreeRTOS/Source/portable/MemMang

CC = gcc

CFLAGS += -Wall -Werror -Wextra -Wpedantic -pthread -O3 -g
LDFLAGS += -Wall -Werror -Wextra -Wpedantic -pthread -O3 -g

INCLUDE_DIRS +=	-I. \
				-I${FREERTOS_INC_DIR} \
				-I${PORTABLE_SRC_DIR} \

VPATH += ${FREERTOS_SRC_DIR} \
		 ${PORTABLE_SRC_DIR} \
		 ${PORTABLE_SRC_MEM_MANG_DIR} \

TARGET = Prac1

default: all

all: ${TARGET}

SRC = \
	  main.c

FREERTOS_SRC= \
			  event_groups.c \
			  list.c \
			  queue.c \
			  tasks.c \
			  timers.c \
			  port.c \
			  heap_3.c \
			  croutine.c \



IMGUI_DIR = GUI
GUI_SOURCES = $(IMGUI_DIR)/GUI.cpp $(IMGUI_DIR)/drivers.cpp $(IMGUI_DIR)/HAL.cpp $(IMGUI_DIR)/Memory.cpp $(IMGUI_DIR)/BSP.cpp
GUI_SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
GUI_SOURCES += $(IMGUI_DIR)/imgui_impl_sdl.cpp $(IMGUI_DIR)/imgui_impl_opengl3.cpp
CXXFLAGS = -I$(IMGUI_DIR)/inc/ -I$(FREERTOS_INC_DIR) -I. -I$(PORTABLE_SRC_DIR)
CXXFLAGS += -g -Wall -Wformat -Wpedantic
GUI_SOURCES += $(IMGUI_DIR)/gl3w.cpp
CXXFLAGS += -I$(IMGUI_DIR)/inc/gl3w -DIMGUI_IMPL_OPENGL_LOADER_GL3W -std=c++11 
LIBS += -lGL -ldl `sdl2-config --libs` 
CXXFLAGS += `sdl2-config --cflags` -pthread -O3 -g -fPIC


OBJDIR= obj
OBJ = ${SRC:%.c=$(OBJDIR)/%.o}

FREERTOS_OBJ = ${FREERTOS_SRC:%.c=$(OBJDIR)/%.o}
GUI_OBJ = ${GUI_SOURCES:%.cpp=$(OBJDIR)/%.o}

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(IMGUI_DIR)/%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<	

${OBJ}: $(OBJDIR)/%.o: %.c
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@ 

${FREERTOS_OBJ}: $(OBJDIR)/%.o: %.c
	${CC} -I. -I${FREERTOS_INC_DIR} -I${PORTABLE_SRC_DIR} ${CFLAGS} -c $< -o $@

${GUI_OBJ}: $(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(TARGET): ${FREERTOS_OBJ} ${OBJ} ${GUI_OBJ}
	${CXX} ${OBJ} ${GUI_OBJ} ${LDFLAGS} ${LD_EXE_FLAGS} -o $@ ${FREERTOS_OBJ}  ${LIBS}  

.PHONY : clean
clean:
	@rm ${OBJ}
	@rm ${FREERTOS_OBJ}
	@rm ${GUI_OBJ}
	@rm ${TARGET}

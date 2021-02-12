/*!
 \file GUI.cpp
 \brief Main GUI 
 \author Màrius Montón
 \date Feb 20021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include <iostream>
#include <SDL.h>



// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else

#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#endif

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "GUI.h"
#include "HAL.h"
#include "BSP.h"
#include "Memory.h"

void gui_thread(void *parameters);


// Main code
extern "C" {

void create_gui() {

    xTaskCreate(gui_thread,
                "GUI",
                10000,
                NULL,
                1,
                NULL);
    }
}

/**
 * Thread with the GUI endless loop.
 * It executes every 100 ms, seems to be enough.
 * @param parameters unused
 */
void gui_thread(void *parameters) {

    (void) parameters;

    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    // Decide GL+GLSL versions
#ifdef __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 500, 500, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

     // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    TickType_t pxPreviousWakeTime;
    int i2c1 = 128;

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        {
            ImGui::Begin("GPIO");
            if (ImGui::Button("Button 1")) {
                memory[ADDR_NVIC_IRQ] |= NVIC_PORTA_IRQ_BIT;
                xSemaphoreGive(GUI_GPIO_IRQ);
            }

            ImGui::SameLine();
            if (ImGui::Button("Button 2")) {
                memory[ADDR_NVIC_IRQ] |= NVIC_PORTB_IRQ_BIT;
                xSemaphoreGive(GUI_GPIO_IRQ);
            }

            ImGui::PushID(0);
            uint32_t LED1 = memory[ADDR_PORTA_CTRL];

            if (LED1 & (1 << 1)) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.3f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0 / 7.0f, 0.3f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0 / 7.0f, 0.3f, 0.3f));
            }

            ImGui::Button("LED 1");
            ImGui::PopStyleColor(3);
            ImGui::PopID();


            ImGui::SameLine();
            ImGui::PushID(1);

            uint32_t LED2 = memory[ADDR_PORTB_CTRL];
            if (LED2 & (1 << 1)) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(3 / 7.0f, 0.3f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(3 / 7.0f, 0.3f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(3 / 7.0f, 0.3f, 0.3f));
            }

            ImGui::Button("LED 2");
            ImGui::PopStyleColor(3);
            ImGui::PopID();

            ImGui::End();

            ImGui::Begin("I2C");

            ImGui::SliderInt("I2C Device #1", &i2c1, 0, 255);
            I2CSlaveSet(1, i2c1);
            ImGui::SameLine();

            ImGui::End();

            ImGui::Begin("Trace");

            ImGui::Text("Trace output");
            ImGui::BeginChild("Scrolling");
            static char str[80];
            static int str_idx = 0;
            char c =  memory[ADDR_TRACE];
            if (c != 0) {
                str[str_idx++] = c;
                memory[ADDR_TRACE] = 0;
                if (str_idx == 80) {
                    str_idx = 0;
                }
            }
            ImGui::Text( str );
            ImGui::EndChild();
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        /* Refresh every 100 ms ? */
        vTaskDelayUntil( &pxPreviousWakeTime, 100);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


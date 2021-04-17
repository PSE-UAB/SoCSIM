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
#include <ctime>
#include <thread>
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

#include "SIM/GUI.h"
#include "SIM/HAL.h"
#include <SIM/SoC.h>
#include "Memory.h"


void *gui_thread(void *ptr);

/**
 * @brief Text buffer to keep the trace output
 */
ImGuiTextBuffer *trace_console;

// Main code
extern "C" {

void gui_create() {

    pthread_t thread1;
    pthread_create (&thread1, nullptr, gui_thread, nullptr);

    trace_console = new ImGuiTextBuffer();
}


}

/**
 * Thread with the GUI endless loop.
 * @param ptr unused
 */
void *gui_thread(void *ptr) {

    (void) ptr;

    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return nullptr;
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
    auto window_flags = (SDL_WindowFlags) (
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 800, 500, window_flags);
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
        return nullptr;
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

    //int i2c1 = 128;



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
            /************** GPIO ***************/
            ImGui::Begin("GPIO");
            if (ImGui::Button("Button 1")) {
                SoC_Button1Pressed();
            } else {
                SoC_Button1Released();
            }

            ImGui::SameLine();
            if (ImGui::Button("Button 2")) {
                SoC_Button2Pressed();
            } else {
                SoC_Button2Released();
            }

            ImGui::PushID(0);

            if (SoC_LED1On() == true) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0, 0.8f, 0.8f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0, 0.3f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0, 0.3f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0, 0.3f, 0.3f));
            }

            ImGui::Button("LED 1");
            ImGui::PopStyleColor(3);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID(1);

            if (SoC_LED2On() == true) {
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

            /************** I2C ***************/
//            ImGui::Begin("I2C");
//            ImGui::SliderInt("I2C Device #1", &i2c1, 0, 255);
//            I2CSlaveSet(1, i2c1);
//            ImGui::SameLine();
//            ImGui::End();

            /************** TRACE ***************/
            ImGui::Begin("Trace");
            ImGui::Text("Trace output");
            ImGui::BeginChild("Scrolling");
            ImGui::Text("%s", trace_console->c_str());
            ImGui::EndChild();
            ImGui::End();

            /************** PWM ***************/
            ImGui::Begin("PWM");
            unsigned int pwmfreq = PWMFreqGet();
            unsigned int pwmduty = PWMDutyGet();
            ImGui::Button("PWM Freq");
            ImGui::SameLine();
            ImGui::Text("%3d Hz", pwmfreq);
            ImGui::Button("PWM Duty");
            ImGui::SameLine();
            ImGui::Text("%3d %%", pwmduty);
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(0, (int) ((255 * (float) pwmduty) / 100.0f), 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                  (ImVec4) ImColor(0, (int) ((255 * (float) pwmduty) / 100.0f), 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  (ImVec4) ImColor(0, (int) ((255 * (float) pwmduty) / 100.0f), 0));
            ImGui::SameLine();
            ImGui::Button("PWM");
            ImGui::PopStyleColor(3);
            ImGui::PopID();
            ImGui::End();

            /************** RTC ***************/
            ImGui::Begin("RTC");
            uint32_t now = RTC_CounterGet();
            struct tm *ptm = localtime((time_t *) &now);
            ImGui::Text("CNT: %u (%02d/%02d/%04d %02d:%02d:%02d)", now, ptm->tm_mday, ptm->tm_mon + 1,
                        ptm->tm_year + 1900,
                        ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
            now = RTC_CompareGet();
            ptm = localtime((time_t *) &now);
            ImGui::Text("CMP: %u (%02d/%02d/%04d %02d:%02d:%02d)", now, ptm->tm_mday, ptm->tm_mon + 1,
                        ptm->tm_year + 1900,
                        ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
            ImGui::End();

            /*************** DAC ***********/
            ImGui::Begin("DAC");
            ImGui::PlotLines("DAC output", get_DACVal, nullptr, DAC_TOTAL_VALUES, 0, nullptr, 0, 4096,
                             ImVec2(0, 80.0f));
            ImGui::End();

            /**************** UART **********/
            ImGui::Begin("UART");
            std::string device = getUART_Path();
            ImGui::Text("Baudrate %d %s", UART_GetBaudRate(), device.c_str());
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return nullptr;
}


void gui_add_trace(char c) {

    if (c != 0) {
        memory[ADDR_TRACE] = 0;
        trace_console->appendf("%c", c);
    }
}

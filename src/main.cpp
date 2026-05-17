#include <SDL.h>
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION 
#endif
#include <SDL_opengl.h>
#include<string>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

enum class MachineState
{
    Idle,
    Working,
    Broken
};

struct MachineSnapshot
{
    std::string name;

    MachineState state;

    float progress;

    float health;

    int queueDepth;
};

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // ============================================================
    // Setup OpenGL Context and GLSL Version per OS
    // ============================================================
    const char* glsl_version;
#if defined(__APPLE__)
    // macOS: Requires OpenGL 3.2 Core Profile and GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // Windows/Linux: Standard OpenGL 3.0 is sufficient
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context (OpenGL flag is required)
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 Setup", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    
    // Create OpenGL Context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable VSync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends (passing the OS-specific glsl_version)
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
 /* End of CROSS-PLATFORM */
    bool running = true;
    
 // MachinSnap 정의   
    MachineSnapshot snap;

    snap.name="Cutter";
    snap.state=MachineState::Idle;
    snap.progress=0.8f;
    snap.health=0.3f;
    snap.queueDepth=3;

    ImVec4 StateColor;
    ImVec4 BarColor;
    const char* text;

    while (running)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        // Your UI goes here
        ImGui::SetNextWindowSize(
            ImVec2(800,400),
            ImGuiCond_FirstUseEver
        );
        ImGui::Begin(snap.name.c_str()); //machine 이름

        //상태 표시 및 색깔
        switch(snap.state)
        {
            case MachineState::Working:
                StateColor=ImVec4(0,1,0,1);
                text="WORKING";
                break;
            case MachineState::Idle:
                StateColor=ImVec4(1,1,1,1);
                text="IDLE";
                break;
            case MachineState::Broken:
                StateColor=ImVec4(1,0,0,1);
                text="BROKEN";
                break;
        }
        ImGui::TextColored(StateColor, text);
        ImGui::Separator();
        
        //ProgressBar작성

        ImGui::Text("Progress");
        ImGui::ProgressBar(snap.progress,ImVec2(0.0f,0.0f));
        ImGui::Spacing();

        //Health Progressbar 작성
        if (snap.health>=0.5f)
        {
            BarColor=ImVec4(0,1,0,1);
        }
        else if (snap.health<0.5f && snap.health>0.3f)
        {
            BarColor=ImVec4(1,0.8f,0,1);
        }
        else
        {
            BarColor=ImVec4(1,0,0,1);
        }

        ImGui::PushStyleColor(
            ImGuiCol_PlotHistogram,
            BarColor
        );

        ImGui::Text("Health");
        ImGui::ProgressBar(snap.health,ImVec2(0.0f,0.0f));

        ImGui::PopStyleColor();
        ImGui::Spacing();
        ImGui::Text("Queue: %d", snap.queueDepth);
        
        //고장, 수리 버튼
        if(ImGui::Button("Break"))
        {
            snap.state=MachineState::Broken;
        }
        ImGui::SameLine();
        if(ImGui::Button("Repair"))
        {
            snap.state=MachineState::Working;
        }
        ImGui::End();
        
        // Render
        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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

    return 0;
}
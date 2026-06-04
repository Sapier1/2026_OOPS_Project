#include <SDL.h>
#include <SDL_opengl.h>
#include <ctime>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "Backend/FactorySimulation.h"
#include "SimulationCmd.h"
#include "UI/UIManager.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char* glsl_version;

    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Factory Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, window_flags);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //특수문자,한글이 깨지지 않도록 NotoSansKR 폰트 추가.
     io.Fonts->AddFontFromFileTTF("Assets/Fonts/NotoSansKR-Bold.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    srand((unsigned int)time(nullptr));

    // 기존: Cutter cutter; MachineController ctrlCutter(&cutter);
    // 변경: FactorySimulation 하나가 전체 파이프라인을 관리한다.
    FactorySimulation factory;

    // 기존에는 main.cpp가 SimulationControlView와 MachineCardView들을 직접 호출했다.
    // 변경 후에는 UIManager가 전체 UI 레이아웃과 하위 View 호출 순서를 관리한다.
    UIManager uiManager;

    // 배속(speed)에 따라 틱 간격 변경
    // speed=1 → 1000ms/틱,  speed=5 → 200ms/틱
    const Uint32 BASE_TICK_MS = 1000;
    Uint32 lastTickTime = SDL_GetTicks();

    bool running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 커맨드 수집
        // UI 담당자가 작성한 render 함수들이 이 cmd에 플래그를 기록한다.
        SimulationCmd cmd;

        FactorySnap snap = factory.getSnapshot();

        // 기존 호출 방식:
        //   ControlView.render(snap.tick, cmd);
        //   renderMachineCard(factory.getCutterCtrl());
        //   renderMachineCard(factory.getAssemblerCtrl());
        //   renderMachineCard(factory.getPainterCtrl());
        //
        // 변경 후 호출 방식:
        //   main.cpp는 UIManager를 한 번만 호출하고,
        //   UIManager 내부에서 각 View와 창 위치를 일괄 관리한다.
        uiManager.renderAll(
            snap,
            factory,
            cmd
        );  

        // 기존: cutter.update(tick++) 등 수동 호출
        // 변경: applyCmd 한 번으로 Start/Pause/Reset/시나리오 모두 처리
        factory.applyCmd(cmd);
        cmd = {};  // 다음 프레임에 중복 적용되지 않도록 클리어

        // 기존: cutter.update(tick++) 직접 호출
        // 변경: RUNNING 상태일 때만 factory.tick()이 파이프라인 전체를 진행
        Uint32 now = SDL_GetTicks();
        Uint32 tickInterval = BASE_TICK_MS / (Uint32)factory.getSpeed();
        if (now - lastTickTime >= tickInterval) {
            factory.tick();
            lastTickTime = now;
        }

        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

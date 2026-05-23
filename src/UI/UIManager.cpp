#include "UI/UIManager.h"

#include "imgui.h"
#include "UI/MachineCardView.h"

void UIManager::renderAll(
    const FactorySnap& snap,
    MachineController& cutterCtrl,
    MachineController& assemblerCtrl,
    MachineController& painterCtrl,
    SimulationCmd& outCmd
)
{
    // UIManager가 각 창의 위치와 크기를 고정해서 main.cpp가 UI 배치를 몰라도 되게 한다.
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, 230), ImGuiCond_Always);
    m_controlView.render(snap.tick, outCmd);

    ImGui::SetNextWindowPos(ImVec2(320, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 210), ImGuiCond_Always);
    renderMachineCard(cutterCtrl);

    ImGui::SetNextWindowPos(ImVec2(580, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 210), ImGuiCond_Always);
    renderMachineCard(assemblerCtrl);

    ImGui::SetNextWindowPos(ImVec2(840, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 210), ImGuiCond_Always);
    renderMachineCard(painterCtrl);
}

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
    ImGui::SetNextWindowSize(ImVec2(1180, 70), ImGuiCond_Always);
    m_controlView.render(snap.tick, outCmd);

    m_statsView.render(snap);

    ImGui::SetNextWindowPos(ImVec2(20, 220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 210), ImGuiCond_Always);
    renderMachineCard(cutterCtrl);

    ImGui::SetNextWindowPos(ImVec2(280, 220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 210), ImGuiCond_Always);
    renderMachineCard(assemblerCtrl);

    ImGui::SetNextWindowPos(ImVec2(540, 220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 210), ImGuiCond_Always);
    renderMachineCard(painterCtrl);

    ImGui::SetNextWindowPos(ImVec2(20, 460), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(760, 160), ImGuiCond_Always);
    m_floorView.render(snap);
}

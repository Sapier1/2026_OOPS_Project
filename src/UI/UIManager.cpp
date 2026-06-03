#include "UI/UIManager.h"

#include "imgui.h"

void UIManager::renderAll(
    const FactorySnap& snap,
    FactorySimulation& factory,
    SimulationCmd& outCmd
)
{
    // UIManager가 각 창의 위치와 크기를 고정해서 main.cpp가 UI 배치를 몰라도 되게 한다.
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1180, 70), ImGuiCond_Always);
    m_controlView.render(snap.tick, outCmd);

    m_statsView.render(snap);
    
    ImGui::SetNextWindowPos(ImVec2(20, 220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
    m_floorView.render(snap, factory, m_selectedMachine);

    ImGui::SetNextWindowPos(ImVec2(440, 220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(340, 400), ImGuiCond_Always);
    m_inspectorView.render(m_selectedMachine);

    ImGui::SetNextWindowPos(ImVec2(20, 630), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1080, 150), ImGuiCond_Always); 
    m_logView.render(snap.recentLogs, outCmd);

}

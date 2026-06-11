#include "UI/FactoryFloorView.h"
#include "Backend/FactorySimulation.h"
#include "MachineController.h"

#include "imgui.h"
#include <string>

namespace
{
void renderConveyorLoad(const char* label, const ConveyorSnap& conveyor)
{
    float fraction = conveyor.capacity > 0 ? static_cast<float>(conveyor.size) / conveyor.capacity : 0.0f;
    std::string overlay = std::to_string(conveyor.size) + " / " + std::to_string(conveyor.capacity);

    ImGui::Text("%s", label);
    // 요구사항 'Conveyor load shown via ImGui::ProgressBar' 충족 및 커스텀 디자인
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.22f, 0.54f, 0.87f, 1.0f));
    ImGui::ProgressBar(fraction, ImVec2(-1.0f, 0.0f), overlay.c_str());
    ImGui::PopStyleColor();
}

void renderMachineEntry(MachineController& controller, MachineController*& currentSelected)
{
    MachineSnap snap = controller.getSnapshot();

    ImVec4 stateColor = ImVec4(1, 1, 1, 1);
    const char* stateText = "IDLE";
    switch (snap.state) {
        case 0: stateColor = ImVec4(1, 1, 1, 1); stateText = "IDLE"; break;
        case 1: stateColor = ImVec4(0, 1, 0, 1); stateText = "WORKING"; break;
        case 2: stateColor = ImVec4(1, 0, 0, 1); stateText = "BROKEN"; break;
        case 3: stateColor = ImVec4(1, 1, 0, 1); stateText = "REPAIRING"; break;
    }

    bool isSelected = (currentSelected == &controller);
    
    if (isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
    }
    
    ImGui::PushStyleColor(ImGuiCol_Text, stateColor);
    
    std::string label = snap.name + " [" + stateText + "]##" + snap.name;
    if (ImGui::Selectable(label.c_str(), isSelected, 0, ImVec2(0, 20))) {
        currentSelected = &controller;
    }
    
    ImGui::PopStyleColor();
    if (isSelected) {
        ImGui::PopStyleColor();
    }

    ImGui::ProgressBar(snap.progress, ImVec2(-1.0f, 0.0f), "Progress");
    
    // 체력 바 추가
    ImVec4 healthColor = (snap.health >= 0.5f) ? ImVec4(0, 1, 0, 1) : 
                         (snap.health > 0.3f) ? ImVec4(1, 0.8f, 0, 1) : ImVec4(1, 0, 0, 1);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, healthColor);
    ImGui::ProgressBar(snap.health, ImVec2(-1.0f, 0.0f), "Health");
    ImGui::PopStyleColor();
}
}

void FactoryFloorView::render(
    const FactorySnap& snap,
    FactorySimulation& factory,
    MachineController*& currentSelected
)
{
    ImGui::Begin("Factory Floor");

    size_t count = factory.getMachineCount();
    for (size_t i = 0; i < count; ++i) {
        MachineController& ctrl = factory.getMachineCtrl(i);
        renderMachineEntry(ctrl, currentSelected);
        ImGui::Spacing();
        
        if (i < count - 1 && i < snap.conveyors.size()) {
            std::string convLabel = "Conveyor " + std::to_string(i + 1);
            renderConveyorLoad(convLabel.c_str(), snap.conveyors[i]);
            ImGui::Spacing();
        }
    }

    ImGui::End();
}

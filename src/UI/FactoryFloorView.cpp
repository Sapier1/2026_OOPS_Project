#include "UI/FactoryFloorView.h"

#include "imgui.h"

namespace
{
void renderConveyorLoad(const char* label, const ConveyorSnap& conveyor)
{
    const float load = conveyor.capacity > 0
        ? static_cast<float>(conveyor.size) / conveyor.capacity
        : 0.0f;

    ImGui::Text("%s", label);
    ImGui::ProgressBar(load, ImVec2(0.0f, 0.0f));
    ImGui::SameLine();
    ImGui::Text("%d/%d", conveyor.size, conveyor.capacity);
}
}

void FactoryFloorView::render(const FactorySnap& snap)
{
    ImGui::Begin("Factory Floor");

    renderConveyorLoad("C1 Cutter -> Assembler", snap.c1);
    ImGui::Spacing();

    renderConveyorLoad("C2 Assembler -> Painter", snap.c2);

    ImGui::End();
}

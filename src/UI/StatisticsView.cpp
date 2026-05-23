#include "UI/StatisticsView.h"

#include "imgui.h"

namespace
{
void renderStatWindow(
    const char* title,
    const char* label,
    int value,
    const ImVec2& pos
)
{
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, 100), ImGuiCond_Always);
    ImGui::Begin(title);

    ImGui::Text("%s", label);
    ImGui::Separator();
    ImGui::SetWindowFontScale(1.8f);
    ImGui::Text("%d", value);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::End();
}
}

void StatisticsView::render(const FactorySnap& snap)
{
    renderStatWindow(
        "Finished Goods",
        "Finished Goods",
        snap.finishedGoods,
        ImVec2(20, 100)
    );

    renderStatWindow(
        "WIP Count",
        "WIP Count",
        snap.wipCount,
        ImVec2(320, 100)
    );

    renderStatWindow(
        "Total Breakdowns",
        "Total Breakdowns",
        snap.totalBreakdowns,
        ImVec2(620, 100)
    );

    renderStatWindow(
        "Lost Products",
        "Lost Products",
        snap.lostProducts,
        ImVec2(920, 100)
    );
}

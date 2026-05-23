#include "UI/StatisticsView.h"

#include "imgui.h"

void StatisticsView::render(const FactorySnap& snap)
{
    ImGui::Begin("Statistics");

    ImGui::Text("Finished Goods: %d", snap.finishedGoods);
    ImGui::Text("WIP Count: %d", snap.wipCount);
    ImGui::Text("Total Breakdowns: %d", snap.totalBreakdowns);
    ImGui::Text("Lost Products: %d", snap.lostProducts);

    ImGui::End();
}

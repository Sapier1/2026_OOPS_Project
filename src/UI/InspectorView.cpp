#include "UI/InspectorView.h"
#include "imgui.h"

void InspectorView::render(MachineController* selectedMachine) {
    ImGui::Begin("Inspector");

    if (selectedMachine == nullptr) 
    {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Click 'Select' on a machine card.");
    } 
    else 
    {
        MachineSnap snap = selectedMachine->getSnapshot();

        // 기계 이름 및 상태
        ImGui::Text("Machine: %s", snap.name.c_str());
        ImGui::Separator();

        ImVec4 stateColor = ImVec4(1, 1, 1, 1);
        const char* text = "IDLE";
        switch (snap.state) {
            case 0: stateColor = ImVec4(1, 1, 1, 1); text = "IDLE"; break;
            case 1: stateColor = ImVec4(0, 1, 0, 1); text = "WORKING"; break;
            case 2: stateColor = ImVec4(1, 0, 0, 1); text = "BROKEN"; break;
            case 3: stateColor = ImVec4(1, 1, 0, 1); text = "REPAIRING"; break;
        }
        ImGui::TextColored(stateColor, "State: %s", text);
        ImGui::Spacing();

        // 상세 정보
        ImGui::Text("Queue: %d / %d", snap.queueSize, snap.queueCapacity);
        ImGui::Text("Total Output: %d", snap.completedCount);
        ImGui::Text("Process Time: %d ticks", snap.processTime);
        ImGui::Spacing();

        // 진행률 (Progress)
        ImGui::Text("Progress");
        ImVec4 progressColor = (snap.state == 3) ? ImVec4(1.0f, 0.85f, 0.0f, 1.0f) : ImVec4(0.22f, 0.54f, 0.87f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, progressColor);
        ImGui::ProgressBar(snap.progress, ImVec2(0.0f, 0.0f));
        ImGui::PopStyleColor();
        ImGui::Spacing();

        // 체력 (Health)
        ImGui::Text("Health");
        ImVec4 healthColor = (snap.health >= 0.5f) ? ImVec4(0, 1, 0, 1) : 
                             (snap.health > 0.3f) ? ImVec4(1, 0.8f, 0, 1) : ImVec4(1, 0, 0, 1);
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, healthColor);
        ImGui::ProgressBar(snap.health, ImVec2(0.0f, 0.0f));
        ImGui::PopStyleColor();
        ImGui::Spacing();

        // 고장 확률 (Breakdown Probability)
        ImGui::Text("Breakdown Prob: %.1f%%", snap.breakdownProb * 100.0f);

        // 강제 고장 및 수리 버튼 (MachineCardView에서 가져온 내용)
        ImGui::Separator();
        if (ImGui::Button("Force Break")) {
            selectedMachine->onForceBreakClicked();
        }
        ImGui::SameLine();
        if (ImGui::Button("Instant Repair")) {
            selectedMachine->onRepairClicked();
        }
    }

    ImGui::End();
}
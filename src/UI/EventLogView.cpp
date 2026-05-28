#include "UI/EventLogView.h"
#include "imgui.h"
#include <string>

void EventLogView::render(const std::vector<EventEntry>& logs, SimulationCmd& outCmd) {
    ImGui::Begin("Event Log");

    // 헤더 및 Clear 버튼 배치
    ImGui::Text("System Events");
    ImGui::SameLine(ImGui::GetWindowWidth() - 70); // 창의 우측 끝으로 버튼 밀기
    if (ImGui::Button("Clear", ImVec2(50, 20))) {
        outCmd.OnClearLogClicked = true;
    }
    
    ImGui::Separator();

    // 스크롤 가능한 자식 창 생성 (테두리 없음, 세로 스크롤바 항상 표시)
    ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    
    for (const auto& entry : logs) {
        
        // 키워드에 따라 텍스트 색상을 다르게 지정
        ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 기본 흰색
        if (entry.message.find("BROKEN") != std::string::npos || entry.message.find("lost") != std::string::npos) {
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // 에러/고장은 빨간색
        } else if (entry.message.find("finished") != std::string::npos || entry.message.find("repaired") != std::string::npos) {
            color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f); // 성공/수리는 초록색
        }

        ImGui::TextColored(color, "%s", entry.message.c_str());
    }

    // 만약 스크롤이 맨 아래에 있다면 새 로그가 들어와도 계속 맨 아래로 자동 스크롤 유지
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}
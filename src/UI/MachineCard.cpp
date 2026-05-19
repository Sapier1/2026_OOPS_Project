#include "UI/MachineCard.h"

#include "imgui.h"

void renderMachineCard(
    MachineController& controller
)
{
     MachineSnap snap =controller.getSnapshot();
     //getSnapshot 함수를 통해 불러오기


    ImVec4 StateColor;
    ImVec4 BarColor;
    const char* text;

    ImGui::Begin(snap.name.c_str()); //machine 이름
    switch(snap.state)
        {
            case 0:
                StateColor=ImVec4(1,1,1,1);
                text="IDLE";
                break;
            case 1:
                StateColor=ImVec4(0,1,0,1);
                text="WORKING";
                break;
            case 2:
                StateColor=ImVec4(1,0,0,1);
                text="BROKEN";
                break;
            case 3:
                StateColor=ImVec4(1,1,0,1);
                text="REPARING";
                break;
        }
        ImGui::TextColored(StateColor, text);
        ImGui::Separator();
        
        //ProgressBar작성

        ImGui::Text("Progress");
        ImGui::ProgressBar(snap.progress,ImVec2(0.0f,0.0f));
        ImGui::Spacing();

        //Health Progressbar 작성
        if (snap.health>=0.5f)
        {
            BarColor=ImVec4(0,1,0,1);
        }
        else if (snap.health<0.5f && snap.health>0.3f)
        {
            BarColor=ImVec4(1,0.8f,0,1);
        }
        else
        {
            BarColor=ImVec4(1,0,0,1);
        }

        ImGui::PushStyleColor(
            ImGuiCol_PlotHistogram,
            BarColor
        );

        ImGui::Text("Health");
        ImGui::ProgressBar(snap.health,ImVec2(0.0f,0.0f));

        ImGui::PopStyleColor();
        ImGui::Spacing();
        ImGui::Text("Queue: %d/%d", snap.queueSize, snap.queueCapacity);
        
        //고장, 수리 버튼
        if(ImGui::Button("Break"))
        {
            controller.onForceBreakClicked();
        }
        ImGui::SameLine();
        if(ImGui::Button("Repair"))
        {
            controller.onRepairClicked();
        }
        ImGui::End();
}
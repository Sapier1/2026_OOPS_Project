#include "SimulationControlView.h"

#include "imgui.h"

void SimulationControlView::render(
    int currentTick,
    SimulationCmd& outCmd
)
{
    ImGui::Begin("Simulation Control");

    if (ImGui::Button("Start"))
    {
        outCmd.OnStartClicked = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Pause"))
    {
        outCmd.OnPauseClicked = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
        outCmd.OnResetClicked = true;
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();

    ImGui::SetNextItemWidth(120.0f);
    if (ImGui::SliderInt("Speed", &m_simSpeed, 1, 5))
    {
        outCmd.speedChanged = true;
        outCmd.newSpeed = m_simSpeed;
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();

    const char* scenarios[] =
    {
        "Normal Flow",
        "Random Breakdown",
        "Overflow",
        "Bottleneck"
    };

    ImGui::SetNextItemWidth(170.0f);
    if (ImGui::Combo(
        "Scenario",
        &m_currentScenarioIdx,
        scenarios,
        IM_ARRAYSIZE(scenarios)
    ))
    {
        outCmd.scenarioChanged = true;
        outCmd.newScenario = m_currentScenarioIdx;
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::Text("Tick: %d", currentTick);

    ImGui::End();
}

#include "SimulationControlView.h"

#include "imgui.h"

void SimulationControlView::render(
    int currentTick,
    SimulationCmd& outCmd
)
{
    ImGui::Begin("Simulation Control");

    // 현재 Tick
    ImGui::Text(
        "Current Tick: %d",
        currentTick
    );

    ImGui::Separator();

    // Start
    if (ImGui::Button("Start"))
    {
        outCmd.OnStartClicked = true;
    }

    ImGui::SameLine();

    // Pause
    if (ImGui::Button("Pause"))
    {
        outCmd.OnPauseClicked = true;
    }

    ImGui::SameLine();

    // Reset
    if (ImGui::Button("Reset"))
    {
        outCmd.OnResetClicked = true;
    }

    ImGui::Separator();

    // Speed Slider
    if (ImGui::SliderInt(
        "Speed",
        &m_simSpeed,
        1,
        5
    ))
    {
        outCmd.speedChanged = true;
        outCmd.newSpeed = m_simSpeed;
    }

    ImGui::Separator();

    // Scenario Dropdown
    const char* scenarios[] =
    {
        "Normal Flow",
        "Random Breakdown"
    };

    if (ImGui::Combo(
        "Scenario",
        &m_currentScenarioIdx,
        scenarios,
        IM_ARRAYSIZE(scenarios)
    ))
    {
        outCmd.scenarioChanged = true;
        outCmd.newScenario =
            m_currentScenarioIdx;
    }

    ImGui::End();
}
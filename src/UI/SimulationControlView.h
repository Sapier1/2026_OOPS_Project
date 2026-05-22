#pragma once

#include "SimulationCmd.h"

class SimulationControlView
{
private:
    int m_currentScenarioIdx = 0;
    int m_simSpeed = 1;

public:
    void render(
        int currentTick,
        SimulationCmd& outCmd
    );
};
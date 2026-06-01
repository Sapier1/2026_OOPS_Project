#pragma once
#include <string>
#include <vector>
using namespace std;

#include "../FactorySnap.h"
#include "../SimulationCmd.h"
#include "PipelineEngine.h"
#include "PipelineBuilder.h"
#include "FactoryStatistics.h"
#include "EventLogger.h"
#include "../IMachineControllerProvider.h"

enum class SimulationState { STOPPED = 0, RUNNING = 1, PAUSED = 2 };

class FactorySimulation : public IMachineControllerProvider {
public:
    FactorySimulation();

    void applyCmd(const SimulationCmd& cmd);
    void tick();

    FactorySnap getSnapshot() const;
    int getSpeed() const { return m_speed; }

    size_t getMachineCount() const;
    MachineController& getMachineCtrl(size_t index);

private:
    SimulationState    m_simState = SimulationState::STOPPED;
    SimulationScenario m_scenario = SimulationScenario::NormalFlow;

    int m_tick  = 0;
    int m_speed = 1;

    PipelineBuilder   m_builder;
    PipelineEngine    m_pipeline;
    FactoryStatistics m_stats;
    EventLogger       m_logger;
};
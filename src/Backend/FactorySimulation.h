#pragma once
#include <string>
#include <vector>
using namespace std;

#include "../FactorySnap.h"
#include "../SimulationCmd.h"
#include "PipelineEngine.h"
#include "FactoryStatistics.h"
#include "EventLogger.h"

enum class SimulationState { STOPPED = 0, RUNNING = 1, PAUSED = 2 };

class FactorySimulation {
public:
    FactorySimulation();

    void applyCmd(const SimulationCmd& cmd);
    void tick();

    FactorySnap getSnapshot() const;

    int getSpeed() const { return m_speed; }

    MachineController& getCutterCtrl();
    MachineController& getAssemblerCtrl();
    MachineController& getPainterCtrl();

private:
    SimulationState m_simState = SimulationState::STOPPED;
    SimulationScenario m_scenario = SimulationScenario::NormalFlow;

    int m_tick = 0;
    int m_speed = 1;

    PipelineEngine    m_pipeline; // 파이프라인 물리 로직
    FactoryStatistics m_stats; // 통계 집계
    EventLogger       m_logger; // 이벤트 로그 관리
};
#pragma once
#include <string>
#include <vector>

#include "../FactorySnap.h"
#include "../MachineController.h"
#include "../SimulationCmd.h"

#include "Cutter.h"
#include "Assembler.h"
#include "Painter.h"
#include "Conveyor.h"

enum class SimulationState { STOPPED = 0, RUNNING = 1, PAUSED = 2 };

struct EventEntry {
    int    tick;
    string message;
};


//    Raw mat. → Cutter → C1 → Assembler → C2 → Painter → C3 → Output
//  main.cpp :
//    1) factory.applyCmd(cmd);   // UI 커맨드 적용
//    2) cmd = {};                // 커맨드 클리어 (중복 방지)
//    3) factory.tick();          // 속도에 맞게 n번 호출
//    4) FactorySnap snap = factory.getSnapshot();
class FactorySimulation {
public:
    FactorySimulation();

    void applyCmd(const SimulationCmd& cmd);
    void tick();

    FactorySnap getSnapshot() const;
    vector<EventEntry> getEventLog() const;
    void clearEventLog();

    int getSpeed() const { return m_speed; }

    MachineController& getCutterCtrl();
    MachineController& getAssemblerCtrl();
    MachineController& getPainterCtrl();

private:
    SimulationState  m_simState = SimulationState::STOPPED;
    SimulationScenario m_scenario = SimulationScenario::NormalFlow;
    int  m_tick  = 0;
    int  m_speed = 1;

    Cutter    m_cutter;
    Assembler m_assembler;
    Painter   m_painter;

    MachineController m_ctrlCutter;
    MachineController m_ctrlAssembler;
    MachineController m_ctrlPainter;

    Conveyor  m_c1;   // Cutter    → Assembler
    Conveyor  m_c2;   // Assembler → Painter
    Conveyor  m_c3;   // Painter   → Output (capacity=1, 즉시 수거)

    // 글로벌 통계
    int m_finishedGoods   = 0;
    int m_lostProducts    = 0;
    int m_totalBreakdowns = 0;

    vector<EventEntry> m_eventLog;

    void applyScenario(SimulationScenario s);
    void stepPipeline();     // 한 틱 분량의 파이프라인 전진
    void autoRepairIfNeeded(); // RandomBreakdown 시나리오: 고장 시 자동 수리
    void log(const string& msg);

    MachineSnap makeMachineSnap(const AbstractMachine& m) const;
};
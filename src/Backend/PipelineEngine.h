#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

#include "AbstractMachine.h"
#include "Conveyor.h"
#include "../MachineController.h"
#include "../MachineSnap.h"
#include "../FactorySnap.h"
#include "../SimulationCmd.h"

struct PipelineStepResult {
    int newBreakdowns = 0;
    int newFinished = 0;
    int newLost = 0;
    vector<string> logs;
};

struct PipelineNode {
    AbstractMachine* machine;
    Conveyor* inputConv;   // nullptr → 원재료 자동 투입
    Conveyor* outputConv;  // nullptr → 완성품 집계
};

class PipelineEngine {
public:
    // PipelineBuilder가 호출 — 기계/컨베이어 외부 주입
    void registerNode(AbstractMachine* m, Conveyor* in, Conveyor* out);

    PipelineStepResult step(int tick, SimulationScenario s);
    void applyScenario(SimulationScenario s);
    void reset();

    vector<MachineSnap> getMachineSnaps() const;
    vector<ConveyorSnap> getConveyorSnaps() const;
    int getWipCount() const;

    size_t getMachineCount() const;
    MachineController& getMachineCtrl(size_t index);

private:
    void stepFeedMachines(SimulationScenario s); // 컨베이어 → 기계
    void stepFlushOutputs(int tick, PipelineStepResult& result); // 기계 출력 → 컨베이어/완성품
    void stepCheckForcedBreaks(int tick, PipelineStepResult& result);
    void stepUpdateMachines(int tick, PipelineStepResult& result);
    void stepAutoRepair(int tick, PipelineStepResult& result);

    vector<PipelineNode> m_nodes;
    vector<MachineController> m_controllers;
    vector<Conveyor*>  m_conveyors;

    static constexpr float PROB_BREAKDOWN = 0.06f;

    MachineSnap  makeMachineSnap(const AbstractMachine& m) const;
    ConveyorSnap makeConveyorSnap(const Conveyor& c) const;
};
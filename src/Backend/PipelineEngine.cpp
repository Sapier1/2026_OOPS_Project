#include "PipelineEngine.h"

void PipelineEngine::registerNode(AbstractMachine* m, Conveyor* in, Conveyor* out) {
    m_nodes.push_back({ m, in, out });
    m_controllers.emplace_back(m, in);

    auto addOnce = [&](Conveyor* c) {
        if (c && find(m_conveyors.begin(), m_conveyors.end(), c) == m_conveyors.end())
            m_conveyors.push_back(c);
    };
    addOnce(in);
    addOnce(out);
}

size_t PipelineEngine::getMachineCount() const { return m_controllers.size(); }
MachineController& PipelineEngine::getMachineCtrl(size_t i) { return m_controllers.at(i); }

// 기존 step()은 약 80줄의 단일 함수로 5가지 책임을 처리했는데,
// 각 단계를 분리하여 가독성 및 테스트 용이성 향상.

PipelineStepResult PipelineEngine::step(int tick, SimulationScenario s) {
    PipelineStepResult result;
 
    stepFeedMachines(s);
    stepFlushOutputs(tick, result);
    stepCheckForcedBreaks(tick, result);
    stepUpdateMachines(tick, result);
    stepAutoRepair(tick, result);
 
    return result;
}

// 1. 컨베이어 → 기계 투입
void PipelineEngine::stepFeedMachines(SimulationScenario s) {
    for (int i = static_cast<int>(m_nodes.size()) - 1; i >= 0; --i) {
        auto& node = m_nodes[i];
        if (node.machine->getState() != MachineState::IDLE) continue;
 
        // Bottleneck 시나리오: 출력 컨베이어가 가득 찼으면 투입 중단
        if (s == SimulationScenario::BottleNeck
            && node.outputConv != nullptr
            && node.outputConv->isFull())
        {
            // 투입하지 않고 IDLE 유지 — 제품 생산 없음
            continue;
        }

        if (node.inputConv == nullptr) {
            // 파이프라인 첫 번째 기계: 원재료 자동 투입
            node.machine->acceptItem();
        } else if (!node.inputConv->isEmpty()) {
            node.inputConv->pop();
            node.machine->acceptItem();
        }
    }
}

// 2. 기계 출력 → 다음 컨베이어 or 완성품
void PipelineEngine::stepFlushOutputs(int tick, PipelineStepResult& result) {
    for (auto& node : m_nodes) {
        if (!node.machine->hasOutputReady()) continue;
        node.machine->collectOutput();
 
        if (node.outputConv == nullptr) {
            ++result.newFinished;
            result.logs.push_back(
                "[Tick " + std::to_string(tick) + "] Finished good #(cumulative)");
        } else if (!node.outputConv->push()) {
            ++result.newLost;
            result.logs.push_back(
                "[Tick " + std::to_string(tick) + "] " +
                node.machine->getMachineName() + " overflow — product lost");
        }
    }
}

// 3. 강제 고장 체크
void PipelineEngine::stepCheckForcedBreaks(int tick, PipelineStepResult& result) {
    for (auto& node : m_nodes) {
        if (!node.machine->wasForcedBreak()) continue;
        ++result.newBreakdowns;
        result.logs.push_back(
            "[Tick " + std::to_string(tick) + "] " +
            node.machine->getMachineName() + " force-broken!");
        node.machine->clearForcedBreak();
    }
}

// 4. 기계 update + 랜덤 고장 체크
void PipelineEngine::stepUpdateMachines(int tick, PipelineStepResult& result) {
    // update() 전 breakdown 카운트 스냅샷
    std::vector<int> prevBreak;
    prevBreak.reserve(m_nodes.size());
    for (auto& node : m_nodes)
        prevBreak.push_back(node.machine->getBreakdownCount());
 
    for (auto& node : m_nodes)
        node.machine->update(tick);
 
    // update() 후 카운트 증가 = 랜덤 고장
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i].machine->getBreakdownCount() > prevBreak[i]) {
            ++result.newBreakdowns;
            result.logs.push_back(
                "[Tick " + std::to_string(tick) + "] " +
                m_nodes[i].machine->getMachineName() + " broken down randomly.");
        }
    }
}

// 5. 자동 수리
void PipelineEngine::stepAutoRepair(int tick, PipelineStepResult& result) {
    for (auto& node : m_nodes) {
        if (node.machine->getState() != MachineState::BROKEN) continue; // ← return → continue
 
        // UI의 forceBreak으로 인한 고장은 사용자가 직접 수리해야 한다
        if (node.machine->wasForcedBreak()) continue;
 
        node.machine->incrementBrokenTicks();
        if (node.machine->getBrokenTicks() < node.machine->getBrokenWaitTime())
            continue;
 
        node.machine->repair();
 
        if (node.machine->isRetired()) {
            result.logs.push_back(
                "[Tick " + std::to_string(tick) + "] WARNING: " +
                node.machine->getMachineName() + " HP critical — needs replacement!");
        }
    }
}

void PipelineEngine::applyScenario(SimulationScenario s) {
    for (auto& node : m_nodes) {
        node.machine->setProcessTime(node.machine->getProcessTime());
    }
    
    switch (s) {
        case SimulationScenario::NormalFlow:
            for (auto& node : m_nodes) {
                node.machine->setBreakdownProb(node.machine->getBreakdownProb());
                node.machine->setProcessTime(node.machine->getProcessTime());
            }
             break;
        case SimulationScenario::RandomBreakdown:
            for (auto& node : m_nodes)
                node.machine->setBreakdownProb(PROB_BREAKDOWN);
             break;
        case SimulationScenario::BottleNeck:
            for (auto& node : m_nodes)
                if (node.machine->getMachineName() == "Assembler")
                    node.machine->setProcessTime(12);
            break;
        case SimulationScenario::OverFlow:
             for (auto& node : m_nodes)
                if (node.machine->getMachineName() == "Assembler")
                    node.machine->setProcessTime(10);
             break;
        default:
            break;
    }
}

vector<MachineSnap> PipelineEngine::getMachineSnaps() const {
    vector<MachineSnap> v;
    for (const auto& node : m_nodes) v.push_back(makeMachineSnap(*node.machine));
    return v;
}

vector<ConveyorSnap> PipelineEngine::getConveyorSnaps() const {
    vector<ConveyorSnap> v;
    for (const auto* c : m_conveyors) v.push_back(makeConveyorSnap(*c));
    return v;
}

int PipelineEngine::getWipCount() const {
    int wip = 0;
    for (const auto& node : m_nodes)
        if (node.machine->getState() == MachineState::WORKING) ++wip;
    for (const auto* c : m_conveyors) wip += c->getSize();
    return wip;
}

void PipelineEngine::reset() {
    for (auto& node : m_nodes) node.machine->reset();
    for (auto* c : m_conveyors) c->reset();
}

MachineSnap PipelineEngine::makeMachineSnap(const AbstractMachine& m) const {
    return {
        m.getMachineName(),
        (int)m.getState(),
        m.getProgress(),
        0,
        5,
        m.getCompletedCount(),
        m.getHealth(),
        m.getProcessTime()
    };
}

ConveyorSnap PipelineEngine::makeConveyorSnap(const Conveyor& c) const {
    return { c.getSize(), c.getCapacity() };
}
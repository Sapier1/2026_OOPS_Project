#include "PipelineEngine.h"

void PipelineEngine::registerNode(AbstractMachine* m, Conveyor* in, Conveyor* out) {
    m_nodes.push_back({ m, in, out });
    m_controllers.emplace_back(m);

    auto addOnce = [&](Conveyor* c) {
        if (c && find(m_conveyors.begin(), m_conveyors.end(), c) == m_conveyors.end())
            m_conveyors.push_back(c);
    };
    addOnce(in);
    addOnce(out);
}

size_t PipelineEngine::getMachineCount() const { return m_controllers.size(); }
MachineController& PipelineEngine::getMachineCtrl(size_t i) { return m_controllers.at(i); }

PipelineStepResult PipelineEngine::step(int tick) {
    PipelineStepResult result;

    // 1. 컨베이어 → 기계 (역순: 하류부터 당겨야 병목 없음)
    for (int i = (int)m_nodes.size() - 1; i >= 0; --i) {
        auto& node = m_nodes[i];
        if (node.machine->getState() != MachineState::IDLE) continue;
        if (node.inputConv == nullptr) {
            node.machine->acceptItem();
        } else if (!node.inputConv->isEmpty()) {
            node.inputConv->pop();
            node.machine->acceptItem();
        }
    }

    // 2. 기계 출력 → 다음 컨베이어 or 완성품
    for (auto& node : m_nodes) {
        if (!node.machine->hasOutputReady()) continue;
        node.machine->collectOutput();

        if (node.outputConv == nullptr) {
            ++result.newFinished;
            result.logs.push_back(
                "[Tick " + to_string(tick) + "] Finished good #(cumulative)");
        } else if (!node.outputConv->push()) {
            ++result.newLost;
            result.logs.push_back(
                "[Tick " + to_string(tick) + "] " +
                node.machine->getMachineName() + " overflow — product lost");
        }
    }

    // 3. 자동 수리
    autoRepair(tick, result);

    // 4. update + 고장 감지
    vector<int> prevBreak;
    for (auto& node : m_nodes)
        prevBreak.push_back(node.machine->getBreakdownCount());

    for (auto& node : m_nodes)
        node.machine->update(tick);

    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i].machine->getBreakdownCount() > prevBreak[i]) {
            ++result.newBreakdowns;
            result.logs.push_back(
                "[Tick " + to_string(tick) + "] " +
                m_nodes[i].machine->getMachineName() + " BROKEN");
        }
    }

    return result;
}

void PipelineEngine::autoRepair(int tick, PipelineStepResult& result) {
    for (auto& node : m_nodes) {
        if (node.machine->getState() == MachineState::BROKEN) {
            node.machine->repair();
            result.logs.push_back(
                "[Tick " + to_string(tick) +
                "] Technician dispatched → " + node.machine->getMachineName());
        }
    }
}

void PipelineEngine::applyScenario(SimulationScenario s) {
    float prob = (s == SimulationScenario::RandomBreakdown)
                     ? PROB_BREAKDOWN : PROB_NORMAL;
    for (auto& node : m_nodes)
        node.machine->setBreakdownProb(prob);
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
    return { m.getMachineName(), (int)m.getState(), m.getProgress(),
             0, 5, m.getCompletedCount(), m.getHealth(), m.getProcessTime() };
}

ConveyorSnap PipelineEngine::makeConveyorSnap(const Conveyor& c) const {
    return { c.getSize(), c.getCapacity() };
}
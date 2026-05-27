#include "PipelineEngine.h"

PipelineEngine::PipelineEngine()
    : m_c1(5), m_c2(5),
      m_ctrlCutter(&m_cutter, nullptr), // Cutter는 원재료 투입구이므로 inputConveyor 없음
      m_ctrlAssembler(&m_assembler, &m_c1),
      m_ctrlPainter(&m_painter, &m_c2)
{
    applyScenario(SimulationScenario::NormalFlow);
}

//  순서: 출력 수거 → 아이템 전달 → 자동 수리 → update()
//  결과(완료/손실/고장/로그)를 PipelineStepResult로 반환
PipelineStepResult PipelineEngine::step(int tick) {
    PipelineStepResult result;

    // 1. 출력 수거 & 다음 단계로 전달

    // Painter 완료 → 완성품
    if (m_painter.hasOutputReady()) {
        m_painter.collectOutput();
        ++result.newFinished;
        result.logs.push_back("[Tick " + to_string(tick) + "] Finished good #(cumulative)");
    }

    // Assembler 완료 → C2 push
    if (m_assembler.hasOutputReady()) {
        m_assembler.collectOutput();
        if (!m_c2.push()) {
            ++result.newLost;
            result.logs.push_back("[Tick " + to_string(tick) + "] C2 overflow — product lost");
        }
    }

    // Cutter 완료 → C1 push
    if (m_cutter.hasOutputReady()) {
        m_cutter.collectOutput();
        if (!m_c1.push()) {
            ++result.newLost;
            result.logs.push_back("[Tick " + to_string(tick) + "] C1 overflow — product lost");
        }
    }

    // 2. 아이템 전달 (컨베이어 → 기계)

    // C2 → Painter
    if (!m_c2.isEmpty() && m_painter.getState() == MachineState::IDLE) {
        m_c2.pop();
        m_painter.acceptItem();
    }

    // C1 → Assembler
    if (!m_c1.isEmpty() && m_assembler.getState() == MachineState::IDLE) {
        m_c1.pop();
        m_assembler.acceptItem();
    }

    // Raw material → Cutter
    if (m_cutter.getState() == MachineState::IDLE) {
        m_cutter.acceptItem();
    }

    autoRepair(tick, result);

    // 4. 각 기계 update — 고장 감지를 위해 업데이트 전후 카운트 비교
    int prevCutterBreak = m_cutter.getBreakdownCount();
    int prevAssemblerBreak = m_assembler.getBreakdownCount();
    int prevPainterBreak = m_painter.getBreakdownCount();

    m_cutter.update(tick);
    m_assembler.update(tick);
    m_painter.update(tick);

    // 5. 새 고장 이벤트 감지 및 결과 기록
    if (m_cutter.getBreakdownCount() > prevCutterBreak) {
        ++result.newBreakdowns;
        result.logs.push_back("[Tick " + to_string(tick) + "] Cutter BROKEN");
    }
    if (m_assembler.getBreakdownCount() > prevAssemblerBreak) {
        ++result.newBreakdowns;
        result.logs.push_back("[Tick " + to_string(tick) + "] Assembler BROKEN");
    }
    if (m_painter.getBreakdownCount() > prevPainterBreak) {
        ++result.newBreakdowns;
        result.logs.push_back("[Tick " + to_string(tick) + "] Painter BROKEN");
    }

    return result;
}


void PipelineEngine::autoRepair(int tick, PipelineStepResult& result) {
    auto tryAutoRepair = [&](AbstractMachine& m, const char* name) {
        if (m.getState() == MachineState::BROKEN) {
            m.repair();
            result.logs.push_back("[Tick " + to_string(tick) +"] Technician dispatched → " + string(name));
        }
    };

    tryAutoRepair(m_cutter, "Cutter");
    tryAutoRepair(m_assembler, "Assembler");
    tryAutoRepair(m_painter, "Painter");
}

void PipelineEngine::applyScenario(SimulationScenario s) {
    float prob = (s == SimulationScenario::RandomBreakdown)
                     ? PROB_BREAKDOWN
                     : PROB_NORMAL;

    m_cutter.setBreakdownProb(prob);
    m_assembler.setBreakdownProb(prob);
    m_painter.setBreakdownProb(prob);
}


PipelineSnap PipelineEngine::getSnap() const {
    PipelineSnap snap;

    snap.cutter = makeMachineSnap(m_cutter);
    snap.assembler = makeMachineSnap(m_assembler);
    snap.painter = makeMachineSnap(m_painter);

    snap.c1 = { m_c1.getSize(), m_c1.getCapacity() };
    snap.c2 = { m_c2.getSize(), m_c2.getCapacity() };

    // WIP = 컨베이어 내 아이템 수 + WORKING 상태 기계 수
    int machineWip = 0;
    if (m_cutter.getState() == MachineState::WORKING) ++machineWip;
    if (m_assembler.getState() == MachineState::WORKING) ++machineWip;
    if (m_painter.getState() == MachineState::WORKING) ++machineWip;
    snap.wipCount = machineWip + m_c1.getSize() + m_c2.getSize();

    return snap;
}

void PipelineEngine::reset() {
    m_cutter.reset();
    m_assembler.reset();
    m_painter.reset();
    m_c1.reset();
    m_c2.reset();
}

MachineController& PipelineEngine::getCutterCtrl()    { return m_ctrlCutter; }
MachineController& PipelineEngine::getAssemblerCtrl() { return m_ctrlAssembler; }
MachineController& PipelineEngine::getPainterCtrl()   { return m_ctrlPainter; }

MachineSnap PipelineEngine::makeMachineSnap(const AbstractMachine& m) const {
    MachineSnap s;
    s.name = m.getMachineName();
    s.state = static_cast<int>(m.getState());
    s.progress = m.getProgress();
    s.health = m.getHealth();
    s.completedCount = m.getCompletedCount();
    s.processTime = m.getProcessTime();
    s.queueSize = 0;
    s.queueCapacity = 5;
    return s;
}
#include "FactorySimulation.h"

static constexpr float PROB_NORMAL    = 0.02f;  // Normal flow
static constexpr float PROB_BREAKDOWN = 0.06f;  // Random Breakdown

FactorySimulation::FactorySimulation()
    : m_c1(5), m_c2(5), m_c3(1),
    m_ctrlCutter(&m_cutter),
    m_ctrlAssembler(&m_assembler),
    m_ctrlPainter(&m_painter)
{
    applyScenario(m_scenario);  // 기본값으로 초기화
}

//  applyCmd  —  UI 커맨드 처리 (매 프레임 1회)
void FactorySimulation::applyCmd(const SimulationCmd& cmd) {

    // ── Reset (최우선 처리) ───────────────────
    if (cmd.OnResetClicked) {
        m_simState = SimulationState::STOPPED;
        m_tick     = 0;

        m_cutter.reset();
        m_assembler.reset();
        m_painter.reset();
        m_c1.reset();
        m_c2.reset();
        m_c3.reset();

        m_finishedGoods   = 0;
        m_lostProducts    = 0;
        m_totalBreakdowns = 0;

        log("── Simulation reset ──");

        // Reset 후 시나리오는 현재 설정 유지, 확률만 재적용
        applyScenario(m_scenario);
        return;  // Reset이면 다른 커맨드는 무시
    }

    // Scenario 변경
    // 시뮬레이션이 멈춰 있을 때만 허용
    if (cmd.scenarioChanged) {
        SimulationScenario newScenario =
            static_cast<SimulationScenario>(cmd.newScenario);

        if (newScenario != m_scenario) {
            m_scenario = newScenario;
            applyScenario(m_scenario);

            const char* names[] = { "Normal Flow", "Random Breakdown" };
            log("Scenario changed → " + string(names[cmd.newScenario]));
        }
    }

    // 배속 변경
    if (cmd.speedChanged && cmd.newSpeed >= 1 && cmd.newSpeed <= 5) {
        m_speed = cmd.newSpeed;
    }

    if (cmd.OnStartClicked) {
        if (m_simState == SimulationState::STOPPED ||
            m_simState == SimulationState::PAUSED)
        {
            m_simState = SimulationState::RUNNING;
            log("Simulation started (tick " + to_string(m_tick) + ")");
        }
    }

    if (cmd.OnPauseClicked) {
        if (m_simState == SimulationState::RUNNING) {
            m_simState = SimulationState::PAUSED;
            log("Simulation paused (tick " + to_string(m_tick) + ")");
        }
    }
}

void FactorySimulation::tick() {
    if (m_simState != SimulationState::RUNNING) return;

    stepPipeline();
    ++m_tick;
}

//  stepPipeline  —  한 틱 분량의 파이프라인 처리
//  순서: 출력 수거 → 아이템 전달 → 자동 수리 → update()
void FactorySimulation::stepPipeline() {

    // 1. 출력 수거 & 다음 단계로 전달

    // Painter 완료 → 완성품
    if (m_painter.hasOutputReady()) {
        m_painter.collectOutput();
        ++m_finishedGoods;
        log("[Tick " + to_string(m_tick) + "] ✓ Finished good #" +
            to_string(m_finishedGoods));
    }

    // Assembler 완료 → C2 push
    if (m_assembler.hasOutputReady()) {
        m_assembler.collectOutput();
        if (!m_c2.push()) {
            ++m_lostProducts;
            log("[Tick " + to_string(m_tick) +
                "] ⚠ C2 overflow — product lost (" +
                to_string(m_lostProducts) + " total)");
        }
    }

    // Cutter 완료 → C1 push
    if (m_cutter.hasOutputReady()) {
        m_cutter.collectOutput();
        if (!m_c1.push()) {
            ++m_lostProducts;
            log("[Tick " + to_string(m_tick) +
                "] ⚠ C1 overflow — product lost (" +
                to_string(m_lostProducts) + " total)");
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

    // Raw material → Cutter (원자재는 항상 공급 가능)
    if (m_cutter.getState() == MachineState::IDLE) {
        m_cutter.acceptItem();
    }

    // 3. 자동 수리 (RandomBreakdown 시나리오)
    autoRepairIfNeeded();

    // 4. 각 기계 update
    int prevCutterBreak    = m_cutter.getBreakdownCount();
    int prevAssemblerBreak = m_assembler.getBreakdownCount();
    int prevPainterBreak   = m_painter.getBreakdownCount();

    m_cutter.update(m_tick);
    m_assembler.update(m_tick);
    m_painter.update(m_tick);

    // 5. 새 고장 이벤트 로그
    if (m_cutter.getBreakdownCount() > prevCutterBreak) {
        ++m_totalBreakdowns;
        log("[Tick " + to_string(m_tick) + "] ✗ Cutter BROKEN");
    }
    if (m_assembler.getBreakdownCount() > prevAssemblerBreak) {
        ++m_totalBreakdowns;
        log("[Tick " + to_string(m_tick) + "] ✗ Assembler BROKEN");
    }
    if (m_painter.getBreakdownCount() > prevPainterBreak) {
        ++m_totalBreakdowns;
        log("[Tick " + to_string(m_tick) + "] ✗ Painter BROKEN");
    }
}

//  RandomBreakdown 시나리오에서 고장난 기계를 자동으로 수리 대기(REPAIRING)로 전환
//  Normal flow에서는 호출되어도 아무 일도 하지 않음
void FactorySimulation::autoRepairIfNeeded() {
    if (m_scenario != SimulationScenario::RandomBreakdown) return;

    auto tryAutoRepair = [&](AbstractMachine& m, const char* name) {
        if (m.getState() == MachineState::BROKEN) {
            m.repair();
            log("[Tick " + to_string(m_tick) +
                "] 🔧 Technician dispatched → " + string(name));
        }
    };

    tryAutoRepair(m_cutter,    "Cutter");
    tryAutoRepair(m_assembler, "Assembler");
    tryAutoRepair(m_painter,   "Painter");
}

//  applyScenario  —  시나리오별 기계 파라미터 설정
void FactorySimulation::applyScenario(SimulationScenario s) {
    float prob = (s == SimulationScenario::RandomBreakdown)
                     ? PROB_BREAKDOWN
                     : PROB_NORMAL;

    m_cutter.setBreakdownProb(prob);
    m_assembler.setBreakdownProb(prob);
    m_painter.setBreakdownProb(prob);
}

//  getSnapshot  —  UI 렌더링용 스냅샷 반환
FactorySnap FactorySimulation::getSnapshot() const {
    FactorySnap snap;

    snap.simState = static_cast<int>(m_simState);
    snap.tick     = m_tick;
    snap.scenario = static_cast<int>(m_scenario);
    snap.speed    = m_speed;

    snap.finishedGoods   = m_finishedGoods;
    snap.lostProducts    = m_lostProducts;
    snap.totalBreakdowns = m_totalBreakdowns;

    // WIP = 컨베이어 내 아이템 수 + 현재 작업 중인 기계 수
    int machineWip = 0;
    if (m_cutter.getState()    == MachineState::WORKING) ++machineWip;
    if (m_assembler.getState() == MachineState::WORKING) ++machineWip;
    if (m_painter.getState()   == MachineState::WORKING) ++machineWip;
    snap.wipCount = machineWip + m_c1.getSize() + m_c2.getSize();

    snap.cutter    = makeMachineSnap(m_cutter);
    snap.assembler = makeMachineSnap(m_assembler);
    snap.painter   = makeMachineSnap(m_painter);

    snap.c1 = { m_c1.getSize(), m_c1.getCapacity() };
    snap.c2 = { m_c2.getSize(), m_c2.getCapacity() };
    snap.c3 = { m_c3.getSize(), m_c3.getCapacity() };

    return snap;
}

//  makeMachineSnap  —  AbstractMachine → MachineSnap 변환
MachineSnap FactorySimulation::makeMachineSnap(const AbstractMachine& m) const {
    MachineSnap s;
    s.name           = m.getMachineName();
    s.state          = static_cast<int>(m.getState());
    s.progress       = m.getProgress();
    s.health         = m.getHealth();
    s.completedCount = m.getCompletedCount();
    s.processTime    = m.getProcessTime();
    // queueSize / queueCapacity는 MachineController 레벨에서는 0 고정
    // FactorySnap의 ConveyorSnap을 통해 제공
    s.queueSize     = 0;
    s.queueCapacity = 5;
    return s;
}

//  이벤트 로그 관리
void FactorySimulation::log(const string& msg) {
    m_eventLog.push_back({ m_tick, msg });
    // 로그가 너무 길어지지 않도록 최대 200개 유지
    if ((int)m_eventLog.size() > 200)
        m_eventLog.erase(m_eventLog.begin());
}

vector<EventEntry> FactorySimulation::getEventLog() const {
    return m_eventLog;
}

void FactorySimulation::clearEventLog() {
    m_eventLog.clear();
}
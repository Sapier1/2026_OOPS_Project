#include "FactorySimulation.h"

FactorySimulation::FactorySimulation() {
    m_pipeline.applyScenario(m_scenario);
}

// applyCmd — UI 커맨드 처리 (매 프레임 1회)
void FactorySimulation::applyCmd(const SimulationCmd& cmd) {

    // Reset (최우선 처리)
    if (cmd.OnResetClicked) {
        m_simState = SimulationState::STOPPED;
        m_tick     = 0;

        m_pipeline.reset();
        m_stats.reset();
        m_logger.clear();

        m_logger.log(0, "── Simulation reset ──");

        // Reset 후 현재 시나리오 재적용
        m_pipeline.applyScenario(m_scenario);
        return;  // Reset이면 다른 커맨드 무시
    }

    if (cmd.scenarioChanged) {
        SimulationScenario newScenario =
            static_cast<SimulationScenario>(cmd.newScenario);

        if (newScenario != m_scenario) {
            m_scenario = newScenario;
            m_pipeline.applyScenario(m_scenario);

            const char* names[] = { "Normal Flow", "Random Breakdown" };
            m_logger.log(m_tick,
                "Scenario changed → " + string(names[cmd.newScenario]));
        }
    }

    if (cmd.speedChanged && cmd.newSpeed >= 1 && cmd.newSpeed <= 5) {
        m_speed = cmd.newSpeed;
    }

    if (cmd.OnStartClicked) {
        if (m_simState == SimulationState::STOPPED ||
            m_simState == SimulationState::PAUSED)
        {
            m_simState = SimulationState::RUNNING;
            m_logger.log(m_tick,
                "Simulation started (tick " + to_string(m_tick) + ")");
        }
    }

    if (cmd.OnPauseClicked) {
        if (m_simState == SimulationState::RUNNING) {
            m_simState = SimulationState::PAUSED;
            m_logger.log(m_tick,
                "Simulation paused (tick " + to_string(m_tick) + ")");
        }
    }
    //EventLog의 clear를 눌렀을 경우
    if (cmd.OnClearLogClicked) {
        m_logger.clear();
    }
}

//  PipelineEngine::step()을 호출하고,
//  반환된 PipelineStepResult를 FactoryStatistics / EventLogger에 위임
void FactorySimulation::tick() {
    if (m_simState != SimulationState::RUNNING) return;

    PipelineStepResult result = m_pipeline.step(m_tick);

    // 통계 집계 위임
    for (int i = 0; i < result.newFinished; ++i) m_stats.recordFinished();
    for (int i = 0; i < result.newLost; ++i) m_stats.recordLost();
    for (int i = 0; i < result.newBreakdowns; ++i) m_stats.recordBreakdown();

    // 완성품 로그에 누적 수 보정 (PipelineEngine은 누적 수를 모름)
    for (const string& msg : result.logs) {
        string final_msg = msg;
        // "Finished good #(cumulative)" 플레이스홀더를 실제 값으로 치환
        const string placeholder = "#(cumulative)";
        size_t pos = final_msg.find(placeholder);
        if (pos != string::npos) {
            final_msg.replace(pos, placeholder.size(), "#" + to_string(m_stats.getFinishedGoods()));
        }
        m_logger.log(m_tick, final_msg);
    }

    ++m_tick;
}

// getSnapshot — UI 렌더링용 스냅샷 조립
FactorySnap FactorySimulation::getSnapshot() const {
    FactorySnap snap;

    // 시뮬레이션 제어 상태
    snap.simState = static_cast<int>(m_simState);
    snap.tick = m_tick;
    snap.scenario = static_cast<int>(m_scenario);
    snap.speed = m_speed;

    // 통계 (FactoryStatistics에서 조립)
    snap.finishedGoods = m_stats.getFinishedGoods();
    snap.lostProducts = m_stats.getLostProducts();
    snap.totalBreakdowns = m_stats.getTotalBreakdowns();

    // 기계 + 컨베이어 + WIP (PipelineEngine에서 조립)
    PipelineSnap psnap = m_pipeline.getSnap();
    snap.wipCount = psnap.wipCount;
    snap.cutter = psnap.cutter;
    snap.assembler = psnap.assembler;
    snap.painter = psnap.painter;
    snap.c1 = psnap.c1;
    snap.c2 = psnap.c2;

    // 이벤트 로그 (EventLogger에서 조립)
    snap.recentLogs = m_logger.getLog();

    return snap;
}

// MachineController 접근자 (PipelineEngine에 위임)
MachineController& FactorySimulation::getCutterCtrl() { return m_pipeline.getCutterCtrl(); }
MachineController& FactorySimulation::getAssemblerCtrl() { return m_pipeline.getAssemblerCtrl(); }
MachineController& FactorySimulation::getPainterCtrl() { return m_pipeline.getPainterCtrl(); }
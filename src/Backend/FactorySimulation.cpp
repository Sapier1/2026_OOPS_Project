#include "FactorySimulation.h"

FactorySimulation::FactorySimulation() {
    m_builder.build(m_pipeline);
    m_pipeline.applyScenario(m_scenario);
}

void FactorySimulation::applyCmd(const SimulationCmd& cmd) {

    // Reset (최우선 처리)
    if (cmd.OnResetClicked) {
        m_simState = SimulationState::STOPPED;
        m_tick     = 0;
        m_pipeline.reset();
        m_stats.reset();
        m_logger.clear();
        m_logger.log(0, "── Simulation reset ──");
        m_pipeline.applyScenario(m_scenario);
        return;
    }

    if (cmd.scenarioChanged) {
        SimulationScenario newScenario = static_cast<SimulationScenario>(cmd.newScenario);

        if (newScenario != m_scenario) {
            m_scenario = newScenario;
            m_pipeline.applyScenario(m_scenario);

            const char* names[] = { "Normal Flow", "Random Breakdown" };
            m_logger.log(m_tick,
                "Scenario changed → " + string(names[cmd.newScenario]));
        }
    }

    if (cmd.speedChanged && cmd.newSpeed >= 1 && cmd.newSpeed <= 5)
        m_speed = cmd.newSpeed;

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
    const string placeholder = "#(cumulative)";
    for (const string& msg : result.logs) {
        string final_msg = msg;
        size_t pos = final_msg.find(placeholder);
        if (pos != string::npos) {
            final_msg.replace(pos, placeholder.size(), "#" + to_string(m_stats.getFinishedGoods()));
        }
        m_logger.log(m_tick, final_msg);
    }

    ++m_tick;
}

size_t FactorySimulation::getMachineCount() const {
    return m_pipeline.getMachineCount();
}
MachineController& FactorySimulation::getMachineCtrl(size_t index) {
    return m_pipeline.getMachineCtrl(index);
}

FactorySnap FactorySimulation::getSnapshot() const {
    FactorySnap snap;

    snap.simState = static_cast<int>(m_simState);
    snap.tick     = m_tick;
    snap.scenario = static_cast<int>(m_scenario);
    snap.speed    = m_speed;

    snap.finishedGoods   = m_stats.getFinishedGoods();
    snap.lostProducts    = m_stats.getLostProducts();
    snap.totalBreakdowns = m_stats.getTotalBreakdowns();

    snap.machines  = m_pipeline.getMachineSnaps();
    snap.conveyors = m_pipeline.getConveyorSnaps();
    snap.wipCount  = m_pipeline.getWipCount();

    snap.recentLogs = m_logger.getLog();

    return snap;
}
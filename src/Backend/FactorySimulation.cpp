#include "FactorySimulation.h"

FactorySimulation::FactorySimulation() {
    m_builder.build(m_pipeline);
    m_pipeline.applyScenario(m_scenario);
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

void FactorySimulation::applyCmd(const SimulationCmd& cmd) {
    if (cmd.OnStartClicked) {
        m_simState = SimulationState::RUNNING;
    }
    if (cmd.OnPauseClicked) {
        m_simState = SimulationState::PAUSED;
    }
    if (cmd.OnResetClicked) {
        m_simState = SimulationState::STOPPED;
        m_tick = 0;
        m_pipeline.reset();
        m_stats.reset();
        m_logger.clear();
    }
    if (cmd.OnClearLogClicked) {
        m_logger.clear();
    }
    if (cmd.scenarioChanged) {
        m_scenario = static_cast<SimulationScenario>(cmd.newScenario);
        m_pipeline.applyScenario(m_scenario);
    }
    if (cmd.speedChanged) {
        m_speed = cmd.newSpeed;
    }
}

void FactorySimulation::tick() {
    if (m_simState == SimulationState::RUNNING) {
        m_tick++;
        PipelineStepResult res = m_pipeline.step(m_tick);
        
        for (int i = 0; i < res.newFinished; ++i) m_stats.recordFinished();
        for (int i = 0; i < res.newLost; ++i) m_stats.recordLost();
        for (int i = 0; i < res.newBreakdowns; ++i) m_stats.recordBreakdown();
        
        for (const string& msg : res.logs) {
            m_logger.log(m_tick, msg);
        }
    }
}
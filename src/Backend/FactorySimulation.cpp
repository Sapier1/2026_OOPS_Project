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

// applyCmd, tick — 기존 코드 그대로 유지
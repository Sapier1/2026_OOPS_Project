#include "AbstractMachine.h"

AbstractMachine::AbstractMachine(int procTime, float breakdownProb, int repairTime)
    : m_processor(procTime)
    , m_health(breakdownProb, repairTime)
    , m_stats()
{}

void AbstractMachine::update(int /*tick*/) {
    switch (m_state) {
        case MachineState::REPAIRING:
            handleRepairingState();
            return;

        case MachineState::BROKEN:
            return;  // repair() 또는 자동 수리(PipelineEngine) 대기

        case MachineState::IDLE:
        case MachineState::WORKING:
            handleWorkingState();
            return;
    }
}

void AbstractMachine::handleRepairingState() {
    if (m_health.tickRepair()) {
        m_health.applyRepairHeal();
        m_state = MachineState::IDLE;
    }
}

void AbstractMachine::handleWorkingState() {
    if (!m_processor.hasItem()) {
        m_state = MachineState::IDLE;
        return;
    }

    // 랜덤 고장 판정
    if (m_health.rollBreakdown()) {
        m_state = MachineState::BROKEN;
        m_health.applyBreakdownDamage();
        m_stats.recordBreakdown();
        // 처리 중 아이템 폐기 — processor 리셋으로 처리
        m_processor.reset();
        return;
    }

    // 정상 작업 진행
    m_state = MachineState::WORKING;
    if (m_processor.tickWork()) {
        m_stats.recordCompletion();
        m_state = MachineState::IDLE;
    }
}

MachineState AbstractMachine::getState() const { return m_state; }

float AbstractMachine::getProgress() const {
    if (m_state == MachineState::REPAIRING)
        return m_health.getRepairProgress();
    return m_processor.getProgress();
}

bool AbstractMachine::acceptItem() {
    if (m_state != MachineState::IDLE) return false;
    if (!m_processor.acceptItem())     return false;
    m_state = MachineState::WORKING;
    return true;
}

bool AbstractMachine::hasOutputReady() const { return m_processor.hasOutputReady(); }
bool AbstractMachine::hasItem()        const { return m_processor.hasItem(); }
void AbstractMachine::collectOutput()        { m_processor.collectOutput(); }
int AbstractMachine::getProcessTime() const {
    return m_processor.getEffectiveProcessTime();
}

float AbstractMachine::getHealth()       const { return m_health.getHealth(); }
float AbstractMachine::getBreakdownProb()const { return m_health.getBreakdownProb(); }
bool  AbstractMachine::wasForcedBreak()  const { return m_health.wasForcedBreak(); }
void  AbstractMachine::clearForcedBreak()      { m_health.clearForcedBreak(); }
bool  AbstractMachine::isRetired()       const { return m_health.isRetired(); }

void AbstractMachine::repair() {
    if (m_state != MachineState::BROKEN) return;
    m_state = MachineState::REPAIRING;
    m_health.startRepair();
    m_processor.reset();          // 고장 중 진행 중이던 작업 초기화
    m_stats.resetBrokenTicks();   // brokenTicks만 초기화, completedCount는 유지
}

void AbstractMachine::forceBreak() {
    m_state = MachineState::BROKEN;
    m_health.forceBreak();          // 내구도 감소 + forcedBreak 플래그
    m_stats.recordBreakdown();
    m_processor.reset();
}

int   AbstractMachine::getCompletedCount()  const { return m_stats.getCompletedCount(); }
int   AbstractMachine::getBreakdownCount()  const { return m_stats.getBreakdownCount(); }
int   AbstractMachine::getBrokenTicks()     const { return m_stats.getBrokenTicks(); }
int   AbstractMachine::getBrokenWaitTime()  const { return MachineStats::BROKEN_WAIT_TIME; }
float AbstractMachine::getBrokenScale()     const { return BROKEN_SCALE; }

void AbstractMachine::incrementBrokenTicks() {
    if (m_state == MachineState::BROKEN)
        m_stats.incrementBrokenTicks();
}

void AbstractMachine::setBreakdownProb(float prob) { m_health.setBreakdownProb(prob); }
void AbstractMachine::setProcessTime(int t)        { m_processor.setProcessTime(t); }
void AbstractMachine::setProcessTimeMultiplier(float multiplier) {
    m_processor.setProcessTimeMultiplier(multiplier);
}

void AbstractMachine::reset() {
    m_state = MachineState::IDLE;
    m_processor.reset();
    m_health.reset();
    m_stats.reset();
}

void AbstractMachine::abortItem() {
    if (m_state != MachineState::WORKING) return;
    m_processor.reset();
    m_state = MachineState::IDLE;
}
#include "AbstractMachine.h"

AbstractMachine::AbstractMachine(int procTime, float prob, int repairTime)
    : m_processTime(procTime)
    , m_breakdownProb(prob)
    , m_repairTime(repairTime) 
{}

void AbstractMachine::update(int tick) {
    updateBase(tick);
}

void AbstractMachine::updateBase(int tick) {
    // 1. 수리 진행
    if (m_state == MachineState::REPAIRING) {
        ++m_repairProgress;
        if (m_repairProgress >= m_repairTime) {
            m_state = MachineState::IDLE;
            m_repairProgress = 0;
        }
        return;
    }
 
    // 2. 고장 상태는 repair() 호출 전까지 대기
    if (m_state == MachineState::BROKEN) return;
 
    // 3. 아이템 없으면 IDLE
    if (!m_hasItem) {
        m_state = MachineState::IDLE;
        return;
    }
 
    // 4. 랜덤 고장 (WORKING 중에만 발생)
    if (static_cast<float>(rand()) / RAND_MAX < m_breakdownProb) {
        m_state = MachineState::BROKEN;
        m_health = std::max(0.0f, m_health - BROKEN_SCALE);
        ++m_breakdownCount;
        m_hasItem = false;  // 처리 중 아이템 폐기
        return;
    }
 
    // 5. 정상 작업 진행
    m_state = MachineState::WORKING;
    ++m_currentProgress;
 
    // 6. 작업 완료
    if (m_currentProgress >= m_processTime) {
        m_currentProgress = 0;
        m_hasItem         = false;
        m_outputReady     = true;
        ++m_completedCount;
        m_state = MachineState::IDLE;
    }
}

MachineState AbstractMachine::getState() const { return m_state; }
float AbstractMachine::getHealth() const { return m_health; }
int AbstractMachine::getProcessTime() const { return m_processTime; }
int AbstractMachine::getCompletedCount() const { return m_completedCount; }
int AbstractMachine::getBreakdownCount() const  { return m_breakdownCount; }
int AbstractMachine::getBrokenTicks() const { return m_brokenTicks; }
int AbstractMachine::getBrokenWaitTime() const { return BROKEN_WAIT_TIME; }
float AbstractMachine::getBrokenScale() const { return BROKEN_SCALE; }
float AbstractMachine::getBreakdownProb() const { return m_breakdownProb; }
float AbstractMachine::getProgress() const {
    if (m_state == MachineState::REPAIRING) {
        return (m_repairTime > 0) ? (float)m_repairProgress / m_repairTime : 0.0f;
    }
    return (m_processTime > 0) ? (float)m_currentProgress / m_processTime : 0.0f;
}

void AbstractMachine::setBreakdownProb(float prob) {
    m_breakdownProb = prob;
}

bool AbstractMachine::wasForcedBreak() const { return m_forcedBreak; }
void AbstractMachine::clearForcedBreak() { m_forcedBreak = false; }

bool AbstractMachine::isRetired() const { return m_health <= 0.0f; }

bool AbstractMachine::acceptItem() {
    if (m_state != MachineState::IDLE || m_hasItem) return false;
    m_hasItem = true;
    m_currentProgress = 0;
    m_state = MachineState::WORKING;
    return true;
}

bool AbstractMachine::hasOutputReady() const { return m_outputReady; }
bool AbstractMachine::hasItem() const { return m_hasItem; }
void AbstractMachine::collectOutput() { m_outputReady = false; }

void AbstractMachine::repair() {
    if (m_state != MachineState::BROKEN) return;
    m_state = MachineState::REPAIRING;
    m_repairProgress = 0;
    m_currentProgress = 0;
    m_hasItem = false;      // 고장 중 처리 중이던 아이템은 폐기
    m_outputReady = false;
    m_health = min(1.0f, m_health + REPAIR_HP_SCALE); // 수리 시 내구도 소량 회복
    m_brokenTicks = 0;
}

void AbstractMachine::forceBreak() {
    m_state = MachineState::BROKEN;
    m_health = max(0.0f, m_health - BROKEN_SCALE); // 고장마다 내구도 scale만큼 감소
    m_hasItem = false;
    m_outputReady = false;
    m_breakdownCount++;
    m_forcedBreak = true;
}

void AbstractMachine::incrementBrokenTicks() {
    if (m_state == MachineState::BROKEN) ++m_brokenTicks;
}

void AbstractMachine::reset() {
    m_state = MachineState::IDLE;
    m_currentProgress = 0;
    m_repairProgress = 0;
    m_health = 1.0f;
    m_hasItem = false;
    m_outputReady = false;
    m_completedCount = 0;
    m_breakdownCount = 0;
    m_forcedBreak = false;
    m_brokenTicks = 0;
}
#include "AbstractMachine.h"

AbstractMachine::AbstractMachine(int procTime, float prob, int repairTime)
    : m_processTime(procTime), m_breakdownProb(prob), m_repairTime(repairTime) {}

MachineState AbstractMachine::getState() const { return m_state; }

float AbstractMachine::getProgress() const {
    if (m_state == MachineState::REPAIRING) {
        // 추가 : 수리 진행률도 0.0~1.0으로 반환
        return (m_repairTime > 0) ? (float)m_repairProgress / m_repairTime : 0.0f;
    }
    return (m_processTime > 0) ? (float)m_currentProgress / m_processTime : 0.0f;
}

float AbstractMachine::getHealth() const { return m_health; }
int AbstractMachine::getProcessTime() const { return m_processTime; }
float AbstractMachine::getBrokenScale() const { return m_brokenScale; }
int AbstractMachine::getCompletedCount() const { return m_completedCount; }
int AbstractMachine::getBreakdownCount() const  { return m_breakdownCount; }
int AbstractMachine::getBrokenTicks() const { return m_brokenTicks; }
int AbstractMachine::getBrokenWaitTime() const { return m_brokenWaitTime; }

void AbstractMachine::setBreakdownProb(float prob) {
    m_breakdownProb = prob;
}

void AbstractMachine::incrementBrokenTicks() {
    if (m_state == MachineState::BROKEN) ++m_brokenTicks;
}
bool AbstractMachine::wasForcedBreak() const { return m_forcedBreak; }
void AbstractMachine::clearForcedBreak() { m_forcedBreak = false; }


bool AbstractMachine::acceptItem() {
    // IDLE 상태이고 현재 처리 중인 아이템이 없을 때만 수락
    if (m_state != MachineState::IDLE || m_hasItem) return false;
    m_hasItem = true;
    m_currentProgress = 0;
    m_state = MachineState::WORKING;
    return true;
}

bool AbstractMachine::hasOutputReady() const {
    return m_outputReady;
}

bool AbstractMachine::hasItem() const {
    return m_hasItem;
}

void AbstractMachine::collectOutput() {
    m_outputReady = false;
}

void AbstractMachine::repair() {
    if (m_state != MachineState::BROKEN) return;
    m_state = MachineState::REPAIRING;
    m_repairProgress = 0;
    m_currentProgress = 0;
    m_hasItem = false;      // 고장 중 처리 중이던 아이템은 폐기
    m_outputReady = false;

    m_health = min(1.0f, m_health + m_repairHpScale); // 수리 시 내구도 소량 회복
    m_brokenTicks = 0;
}

void AbstractMachine::forceBreak() {
    m_state = MachineState::BROKEN;
    m_health = max(0.0f, m_health - getBrokenScale()); // 고장마다 내구도 scale만큼 감소
    m_hasItem = false;
    m_outputReady = false;
    m_breakdownCount++;
    m_forcedBreak = true;
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
#include "MachineHealth.h"
#include <algorithm>
#include <cstdlib>

using std::max;
using std::min;

MachineHealth::MachineHealth(float breakdownProb, int repairTime)
    : m_breakdownProb(breakdownProb)
    , m_repairTime(repairTime)
{}

bool MachineHealth::rollBreakdown() const {
    return static_cast<float>(rand()) / RAND_MAX < m_breakdownProb;
}

void MachineHealth::startRepair() {
    m_repairProgress = 0;
}

bool MachineHealth::tickRepair() {
    ++m_repairProgress;
    if (m_repairProgress >= m_repairTime) {
        m_repairProgress = 0;
        return true;   // 수리 완료
    }
    return false;
}

void MachineHealth::forceBreak() {
    m_forcedBreak = true;
    applyBreakdownDamage();
}

void MachineHealth::applyBreakdownDamage() {
    m_health = max(0.0f, m_health - BROKEN_SCALE);
}

void MachineHealth::applyRepairHeal() {
    m_health = min(1.0f, m_health + REPAIR_HP_SCALE);
}

float MachineHealth::getHealth() const { return m_health; }
float MachineHealth::getBreakdownProb() const { return m_breakdownProb; }
int   MachineHealth::getRepairTime() const { return m_repairTime; }
bool  MachineHealth::wasForcedBreak() const { return m_forcedBreak; }
bool  MachineHealth::isRetired() const { return m_health <= 0.0f; }

float MachineHealth::getRepairProgress() const {
    if (m_repairTime <= 0) return 0.0f;
    return static_cast<float>(m_repairProgress) / m_repairTime;
}

void MachineHealth::clearForcedBreak() { m_forcedBreak = false; }

void MachineHealth::setBreakdownProb(float prob) {
    m_breakdownProb = prob;
}

void MachineHealth::reset() {
    m_health = 1.0f;
    m_repairProgress = 0;
    m_forcedBreak = false;
}
#include "MachineStats.h"

void MachineStats::recordCompletion() { ++m_completedCount; }
void MachineStats::recordBreakdown() { ++m_breakdownCount; }
void MachineStats::incrementBrokenTicks() { ++m_brokenTicks; }

int MachineStats::getCompletedCount() const { return m_completedCount; }
int MachineStats::getBreakdownCount() const { return m_breakdownCount; }
int MachineStats::getBrokenTicks() const { return m_brokenTicks; }

void MachineStats::resetBrokenTicks() { m_brokenTicks = 0; }

void MachineStats::reset() {
    m_completedCount = 0;
    m_breakdownCount = 0;
    m_brokenTicks = 0;
}
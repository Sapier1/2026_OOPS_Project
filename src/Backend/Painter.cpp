#include "Painter.h"
#include <cstdlib>

REGISTER_MACHINE(Painter)

Painter::Painter() : AbstractMachine(4, 0.02f, 5) {}
// 4틱 작업, 2% 고장 확률, 5틱 수리 시간

string Painter::getMachineName() const { return "Painter-01"; }

void Painter::update(int tick) {
    // REPAIRING Process
    if (m_state == MachineState::REPAIRING) {
        m_repairProgress++;
        if (m_repairProgress >= m_repairTime) {
            m_state = MachineState::IDLE;
            m_repairProgress = 0;
        }
        return;
    }

    if (m_state == MachineState::BROKEN) return;

    if (!m_hasItem) {
        m_state = MachineState::IDLE;
        return;
    }

    // Random Breakdown (only during WORKING)
    if (((float)rand() / RAND_MAX) < m_breakdownProb) {
        m_state = MachineState::BROKEN;
        m_health = max(0.0f, m_health - m_brokenScale);
        m_breakdownCount++;
        m_hasItem = false;
        return;
    }

    // Normal operation progress
    m_state = MachineState::WORKING;
    m_currentProgress++;

    // Task completion
    if (m_currentProgress >= m_processTime) {
        m_currentProgress = 0;
        m_hasItem = false;
        m_outputReady = true;
        m_completedCount++;
        m_state = MachineState::IDLE;
    }
}
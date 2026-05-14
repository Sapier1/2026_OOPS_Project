#include "Cutter.h"
#include <cstdlib>

Cutter::Cutter() : AbstractMachine(3, 0.02f) {} // 3틱 소요, 2% 고장 확률

string Cutter::getMachineName() const { return "Sheet Metal Cutter"; }

void Cutter::update(int tick) {
    if (m_state == MachineState::BROKEN) return;

    // 무작위 고장 시나리오 (Random Breakdowns)
    if (((float)rand() / RAND_MAX) < m_breakdownProb) {
        m_state = MachineState::BROKEN;
        return;
    }

    // 정상 흐름 시나리오 (Normal Flow)
    m_state = MachineState::WORKING;
    m_currentProgress++;

    if (m_currentProgress >= m_processTime) {
        m_currentProgress = 0;
        m_state = MachineState::IDLE;
    }
}
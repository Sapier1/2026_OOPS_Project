#include "Painter.h"
#include <cstdlib>

Painter::Painter() : AbstractMachine(5, 0.04f) {} // 5틱 소요, 4% 고장 확률

string Painter::getMachineName() const { return "Car Body Painter"; }

void Painter::update(int tick) {
    if (m_state == MachineState::BROKEN) return;

    if (((float)rand() / RAND_MAX) < m_breakdownProb) {
        m_state = MachineState::BROKEN;
        return;
    }

    m_state = MachineState::WORKING;
    m_currentProgress++;

    if (m_currentProgress >= m_processTime) {
        m_currentProgress = 0;
        m_state = MachineState::IDLE;
    }
}
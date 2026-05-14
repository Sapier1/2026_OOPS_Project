#include "AbstractMachine.h"

AbstractMachine::AbstractMachine(int procTime, float prob) 
    : m_processTime(procTime), m_breakdownProb(prob) {}

MachineState AbstractMachine::getState() const { return m_state; }

float AbstractMachine::getProgress() const { 
    return (m_processTime > 0) ? (float)m_currentProgress / m_processTime : 0.0f; 
}

void AbstractMachine::repair() { 
    m_state = MachineState::IDLE; 
    m_currentProgress = 0; 
}

void AbstractMachine::forceBreak() { m_state = MachineState::BROKEN; }
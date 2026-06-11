#include "MachineController.h"

MachineController::MachineController(
    AbstractMachine* machine, 
    const Conveyor* inputConveyor)
: m_machine(machine),
  m_inputConveyor(inputConveyor)
{}

void MachineController::onRepairClicked() {
    if (m_machine) m_machine->repair();
}

void MachineController::onForceBreakClicked() {
    if (m_machine) m_machine->forceBreak();
}

MachineSnap MachineController::getSnapshot() const {
    if (!m_machine) return { "Unknown", 0, 0.0f };

    int queueSize = m_inputConveyor
        ? m_inputConveyor->getSize()
        : (m_machine->hasItem() ? 1 : 0);
 
    int queueCapacity = m_inputConveyor
        ? m_inputConveyor->getCapacity()
        : 1;

    return {
        m_machine->getMachineName(),
        (int)m_machine->getState(),
        m_machine->getProgress(),
        queueSize,
        queueCapacity,
        m_machine->getCompletedCount(),
        m_machine->getHealth(),
        m_machine->getProcessTime(),
        m_machine->getBreakdownProb()
    };
}
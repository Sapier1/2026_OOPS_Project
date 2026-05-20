#include "MachineController.h"

MachineController::MachineController(AbstractMachine* machine) : m_machine(machine) {}

void MachineController::onRepairClicked() {
    if (m_machine) m_machine->repair();
}

void MachineController::onForceBreakClicked() {
    if (m_machine) m_machine->forceBreak();
}

MachineSnap MachineController::getSnapshot() const {
    if (!m_machine) return { "Unknown", 0, 0.0f };
    return {
        m_machine->getMachineName(),
        (int)m_machine->getState(),
        m_machine->getProgress(),
        0,                           // queueSize (현재 미구현)
        5,                           // queueCapacity (기본값)
        m_machine->getCompletedCount(),
        m_machine->getHealth(),
        m_machine->getProcessTime()
    };
}
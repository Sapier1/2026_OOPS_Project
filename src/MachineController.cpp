#include "MachineController.h"

MachineController::MachineController(AbstractMachine* machine, const Conveyor* inputConveyor)
: m_machine(machine), m_inputConveyor(inputConveyor) {}

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
        m_inputConveyor ? m_inputConveyor->getSize() : 0, // queueSize
        5, // queueCapacity (기본값)
        m_machine->getCompletedCount(),
        m_machine->getHealth(),
        m_machine->getProcessTime()
    };
}
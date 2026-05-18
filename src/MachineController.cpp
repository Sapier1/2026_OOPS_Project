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
        m_machine->getCompletedCount(),
        m_machine->getHealth(),
        m_machine->getProcessTime()
        //queueSize는 FactorySimulation에서 관리하는 별도의 대기열이 필요하므로, 현재는 0으로 고정
    };
}
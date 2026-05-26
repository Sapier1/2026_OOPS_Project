#pragma once
#include "Backend/AbstractMachine.h"
#include "Backend/Conveyor.h"
#include "MachineSnap.h"

class MachineController {
private:
    AbstractMachine* m_machine;
    const Conveyor* m_inputConveyor;
public:
    MachineController(AbstractMachine* machine, const Conveyor* inputConveyor = nullptr);
    
    // UI 이벤트 대응 함수
    void onRepairClicked();
    void onForceBreakClicked();

    // UI 렌더링용 데이터 제공
    MachineSnap getSnapshot() const;
};
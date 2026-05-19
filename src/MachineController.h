#pragma once
#include "MachineController.h"
#include "MachineSnap.h"
#include "Backend/AbstractMachine.h"


class MachineController {
private:
    AbstractMachine* m_machine;

public:
    MachineController(AbstractMachine* machine);
    
    // UI 이벤트 대응 함수
    void onRepairClicked();
    void onForceBreakClicked();

    // UI 렌더링용 데이터 제공
    MachineSnap getSnapshot() const;
};
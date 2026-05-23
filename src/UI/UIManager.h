#pragma once

#include "FactorySnap.h"
#include "MachineController.h"
#include "SimulationCmd.h"
#include "SimulationControlView.h"

class UIManager
{
private:
    SimulationControlView m_controlView;

public:
    // main.cpp에서 UIManager를 한 번만 호출하면,
    // 이 함수가 전체 UI 창 배치와 하위 View 렌더 호출을 담당한다.
    void renderAll(
        const FactorySnap& snap,
        MachineController& cutterCtrl,
        MachineController& assemblerCtrl,
        MachineController& painterCtrl,
        SimulationCmd& outCmd
    );
};

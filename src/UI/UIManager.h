#pragma once

#include "FactorySnap.h"
#include "FactoryFloorView.h"
#include "MachineController.h"
#include "SimulationCmd.h"
#include "SimulationControlView.h"
#include "StatisticsView.h"
#include "InspectorView.h"
#include "EventLogView.h"

class FactorySimulation;

class UIManager
{
private:
    SimulationControlView m_controlView;
    FactoryFloorView m_floorView;
    StatisticsView m_statsView;
    InspectorView m_inspectorView;
    MachineController* m_selectedMachine = nullptr;
    EventLogView m_logView;


public:
    // main.cpp에서 UIManager를 한 번만 호출하면,
    // 이 함수가 전체 UI 창 배치와 하위 View 렌더 호출을 담당한다.
    void renderAll(
        const FactorySnap& snap,
        FactorySimulation& factory,
        SimulationCmd& outCmd
    );
};

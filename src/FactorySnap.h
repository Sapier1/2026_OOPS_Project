#pragma once
#include "MachineSnap.h"
#include "Backend/EventLogger.h" // EventEntry, recentLogs 포함을 위해
#include "Backend/PipelineEngine.h" // ConveyorSnap, PipelineSnap 재사용

struct FactorySnap {
    int simState = 0; // 0=STOPPED, 1=RUNNING, 2=PAUSED
    int tick  = 0;
    int scenario = 0; // SimulationScenario 캐스팅 값
    int speed = 1; // 현재 배속

    int finishedGoods = 0;
    int wipCount = 0;
    int totalBreakdowns = 0;
    int lostProducts = 0;

    MachineSnap cutter;
    MachineSnap assembler;
    MachineSnap painter;

    ConveyorSnap c1; // Cutter → Assembler
    ConveyorSnap c2; // Assembler → Painter

    vector<EventEntry> recentLogs;
};
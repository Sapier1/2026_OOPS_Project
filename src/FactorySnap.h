#pragma once
#include "MachineSnap.h"

//  컨베이어 상태
struct ConveyorSnap {
    int size = 0;
    int capacity = 5;
};

//  Factory 전체 상태
struct FactorySnap {
    // 시뮬레이션 제어 상태
    int  simState = 0; // 0=STOPPED, 1=RUNNING, 2=PAUSED
    int  tick = 0;
    int  scenario = 0; // SimulationScenario 캐스팅 값
    int  speed = 1; // 현재 배속

    // 글로벌 통계
    int  finishedGoods = 0;
    int  wipCount = 0; // 현재 파이프라인 내 작업 중 아이템
    int  totalBreakdowns = 0;
    int  lostProducts = 0; // 컨베이어 포화로 손실된 제품

    MachineSnap cutter;
    MachineSnap assembler;
    MachineSnap painter;

    ConveyorSnap c1; // Cutter → Assembler
    ConveyorSnap c2; // Assembler → Painter
    ConveyorSnap c3; // Painter → Output
};
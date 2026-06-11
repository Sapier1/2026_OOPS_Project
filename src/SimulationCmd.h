#pragma once

//  UI 레이어가 버튼 클릭 결과를 여기에 기록하고,
//  main.cpp가 이 구조체를 factory.applyCmd(cmd)로 전달한다.
//  전달 후 즉시 cmd = {} 로 초기화해야 중복 적용을 막는다.

enum class SimulationScenario {
    NormalFlow      = 0, // 고장 확률 2% (기본값)
    RandomBreakdown = 1, // 고장 확률 6%, 자동 수리
    OverFlow        = 2, // 컨베이어 포화 시 아이템 손실
    BottleNeck      = 3, // Assembler 작업 시간 12틱으로 증가
};

struct SimulationCmd {
    bool OnStartClicked = false;
    bool OnPauseClicked = false;  
    bool OnResetClicked = false;
    bool OnClearLogClicked = false;

    bool scenarioChanged = false;
    int  newScenario = 0; // SimulationScenario 캐스팅 값

    bool speedChanged = false;
    int  newSpeed = 1; // 배속
};
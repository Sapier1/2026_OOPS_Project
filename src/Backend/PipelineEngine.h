#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Cutter.h"
#include "Assembler.h"
#include "Painter.h"
#include "Conveyor.h"
#include "../MachineController.h"
#include "../MachineSnap.h"
#include "../SimulationCmd.h"


//  PipelineStepResult  —  한 틱 결과 요약
//  PipelineEngine::step()이 반환하며,
//  FactorySimulation이 이를 받아 통계/로그에 기록한다.
struct PipelineStepResult {
    int newBreakdowns = 0;
    int newFinished   = 0;
    int newLost       = 0;
    vector<string> logs;   // 이 틱에서 발생한 로그 메시지들
};

struct ConveyorSnap {
    int size     = 0;
    int capacity = 5;
};

struct PipelineSnap {
    MachineSnap  cutter;
    MachineSnap  assembler;
    MachineSnap  painter;

    ConveyorSnap c1;
    ConveyorSnap c2;
    ConveyorSnap c3;
    
    int wipCount = 0;  // 컨베이어 아이템 수 + WORKING 기계 수
};

// ─────────────────────────────────────────────
//  PipelineEngine  —  파이프라인 물리 로직
//
//  책임:
//    - 기계 3대 + 컨베이어 3개 소유
//    - step()으로 한 틱 분량의 아이템 흐름 처리
//    - 시나리오에 따른 고장 확률 + 자동수리 여부 설정
//    - MachineController 제공 (UI 버튼 이벤트 수신용)
//
//  책임 밖:
//    - 시뮬레이션 생애주기 (start/pause/reset) → FactorySimulation
//    - 통계 집계                               → FactoryStatistics
//    - 로그 기록                               → EventLogger
// ─────────────────────────────────────────────
class PipelineEngine {
public:
    PipelineEngine();

    // 한 틱 진행. tick은 로그 메시지 생성에만 사용된다.
    PipelineStepResult step(int tick);

    // 시나리오 변경 시 고장 확률 및 자동수리 여부 재설정
    void applyScenario(SimulationScenario s);

    // UI 렌더링용 스냅샷
    PipelineSnap getSnap() const;

    // 전체 초기화 (기계 + 컨베이어)
    void reset();

    // UI 버튼 이벤트 수신용 컨트롤러
    MachineController& getCutterCtrl();
    MachineController& getAssemblerCtrl();
    MachineController& getPainterCtrl();

private:
    Cutter    m_cutter;
    Assembler m_assembler;
    Painter   m_painter;

    Conveyor  m_c1; // Cutter    → Assembler
    Conveyor  m_c2; // Assembler → Painter

    MachineController m_ctrlCutter;
    MachineController m_ctrlAssembler;
    MachineController m_ctrlPainter;
    
    static constexpr float PROB_NORMAL    = 0.02f;
    static constexpr float PROB_BREAKDOWN = 0.06f;

    // RandomBreakdown 시나리오에서 고장 기계 자동 수리 전환
    void autoRepair(int tick, PipelineStepResult& result);

    MachineSnap makeMachineSnap(const AbstractMachine& m) const;
};
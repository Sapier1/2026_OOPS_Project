#include "Cutter.h"
#include <cstdlib>

Cutter::Cutter() : AbstractMachine(3, 0.02f, 4) {}
// 3틱 작업, 2% 고장 확률, 4틱 수리 시간

string Cutter::getMachineName() const { return "Sheet Metal Cutter"; }

void Cutter::update(int tick) {
    // 1. REPAIRING 처리 — 수리 진행
    if (m_state == MachineState::REPAIRING) {
        m_repairProgress++;
        if (m_repairProgress >= m_repairTime) {
            m_state = MachineState::IDLE;
            m_repairProgress = 0;
        }
        return;
    }

    // 2. 고장 상태면 스킵
    if (m_state == MachineState::BROKEN) return;

    // 3. 처리할 아이템이 없으면 IDLE 유지
    if (!m_hasItem) {
        m_state = MachineState::IDLE;
        return;
    }

    // 4. 랜덤 고장 (작업 중에만 발생)
    // (float)rand() / RAND_MAX는 0.0~1.0 사이의 랜덤 실수 생성
    if (((float)rand() / RAND_MAX) < m_breakdownProb) {
        m_state = MachineState::BROKEN;
        m_health = max(0.0f, m_health - m_brokenScale); 
        m_breakdownCount++;
        m_hasItem = false;  // 처리 중 아이템 폐기
        return;
    }

    // 5. 정상 작업 진행
    m_state = MachineState::WORKING;
    m_currentProgress++;

    // 6. 작업 완료
    if (m_currentProgress >= m_processTime) {
        m_currentProgress = 0;
        m_hasItem = false;
        m_outputReady = true;   // [추가] 다음 컨베이어 전달 신호
        m_completedCount++;     // [추가] 완료 카운트 증가
        m_state = MachineState::IDLE;
    }
}
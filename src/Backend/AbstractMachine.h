#pragma once
#include "SimulationObject.h"
#include "MachineRegistry.h"

enum class MachineState { IDLE, WORKING, BROKEN, REPAIRING };

class AbstractMachine : public SimulationObject {
protected:
    MachineState m_state = MachineState::IDLE;

    int m_processTime;
    int m_currentProgress = 0;
    float m_breakdownProb;
    float m_health = 1.0f; // 0.0~1.0, 고장마다 감소
    const float m_repairHpScale = 0.05f; // 수리 시 회복량

    bool m_hasItem = false; // 현재 처리 중인 아이템 보유 여부
    bool m_outputReady = false; // 완료된 아이템이 출력 대기 중
    bool m_forcedBreak = false; // 강제 고장 여부

    int m_repairTime = 5;
    int m_repairProgress = 0;
    
    int m_completedCount = 0;
    int m_breakdownCount = 0;

    int m_brokenTicks = 0; // BROKEN 상태로 머문 틱 수
    int m_brokenWaitTime = 2; // 이 틱 수가 지나야 repair() 가능

    const float m_brokenScale = 0.1f; // 고장 시 내구도 감소량

public:
    // repairTime: REPAIRING 상태에서 IDLE로 복귀까지 걸리는 틱 수
    AbstractMachine(int procTime, float prob, int repairTime = 5);

    virtual string getMachineName() const = 0;

    MachineState getState() const;
    float getProgress() const; // 작업 or 수리 진행률 0.0~1.0
    float getHealth() const; // 기계 상태 (0.0~1.0)
    int getProcessTime() const; // 4 ticks 표시용
    float getBrokenScale() const;
    int getCompletedCount() const;
    int getBreakdownCount() const;
    int getBrokenTicks() const;
    int getBrokenWaitTime() const;

    // Call when FactorySimulation changes scenario
    void setBreakdownProb(float prob);

    void incrementBrokenTicks();
    bool wasForcedBreak() const;
    void clearForcedBreak();
    bool isRetired() const; // 내구도 0.0 도달 여부 -> 자동 수리

    bool acceptItem();
    bool hasOutputReady() const;
    bool hasItem() const;
    void collectOutput();

    void repair();
    void forceBreak();
    void reset();
};
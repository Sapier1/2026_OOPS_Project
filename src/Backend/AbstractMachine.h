#pragma once
#include "SimulationObject.h"

enum class MachineState { IDLE, WORKING, BROKEN, REPAIRING };

class AbstractMachine : public SimulationObject {
protected:
    MachineState m_state = MachineState::IDLE;

    int m_processTime;
    int m_currentProgress = 0;
    float m_breakdownProb;
    float m_health = 1.0f; // 0.0~1.0, 고장마다 감소

    bool m_hasItem = false; // 현재 처리 중인 아이템 보유 여부
    bool m_outputReady = false; // 완료된 아이템이 출력 대기 중

    int m_repairTime = 5;
    int m_repairProgress = 0;
    
    int m_completedCount = 0;
    int m_breakdownCount = 0;

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

    // Call when FactorySimulation changes scenario
    void setBreakdownProb(float prob);

    bool acceptItem();
    bool hasOutputReady() const;
    bool hasItem() const;
    void collectOutput();

    void repair();
    void forceBreak();
    void reset();
};
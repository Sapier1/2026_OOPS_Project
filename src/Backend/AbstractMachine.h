#pragma once
#include "SimulationObject.h"
#include "MachineRegistry.h"
#include "MachineProcessor.h"
#include "MachineHealth.h"
#include "MachineStats.h"

enum class MachineState { IDLE, WORKING, BROKEN, REPAIRING };

class AbstractMachine : public SimulationObject {
public:
    AbstractMachine(int procTime, float breakdownProb, int repairTime = 5);
    virtual string getMachineName() const = 0;
    void update(int tick) override;

    // 상태 / 진행률
    MachineState getState() const;
    float getProgress() const; // 작업 또는 수리 진행률 0.0~1.0

    // MachineProcessor 위임
    bool acceptItem();
    bool hasOutputReady() const;
    bool hasItem() const;
    void collectOutput();
    int  getProcessTime() const;

    // MachineHealth 위임
    float getHealth() const;
    float getBreakdownProb() const;
    bool wasForcedBreak() const;
    void clearForcedBreak();
    bool isRetired()  const;

    void forceBreak();
    void repair();

    // MachineStats 위임 
    int  getCompletedCount() const;
    int  getBreakdownCount() const;
    int  getBrokenTicks() const;
    int  getBrokenWaitTime() const;
    float getBrokenScale() const;

    void incrementBrokenTicks();
    void abortItem();

    // 시나리오 변경
    void setBreakdownProb(float prob);
    void setProcessTime(int procTime);
    void setProcessTimeMultiplier(float multiplier);

    void reset();

protected:
    MachineState m_state = MachineState::IDLE;

    MachineProcessor m_processor;
    MachineHealth m_health;
    MachineStats m_stats;

private:
    void handleRepairingState();
    void handleWorkingState();

    static constexpr float BROKEN_SCALE = 0.1f;
};
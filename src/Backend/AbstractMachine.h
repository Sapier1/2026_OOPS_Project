#pragma once
#include "SimulationObject.h"

enum class MachineState { IDLE, WORKING, BROKEN, REPAIRING };

class AbstractMachine : public SimulationObject {
protected:
    MachineState m_state = MachineState::IDLE;
    int m_processTime;       // 작업 소요 시간
    int m_currentProgress = 0; 
    float m_breakdownProb;   // 고장 확률

public:
   AbstractMachine(int procTime, float prob);
    virtual string getMachineName() const = 0;
    
    // 캡슐화를 위한 Getter
    MachineState getState() const;
    float getProgress() const;
    
    void repair();     // 수리 로직
    void forceBreak(); // 강제 고장
};
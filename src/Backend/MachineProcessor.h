#pragma once

class MachineProcessor {
public:
    explicit MachineProcessor(int processTime);

    bool acceptItem();
    bool tickWork();
    void collectOutput();

    bool hasItem() const;
    bool hasOutputReady() const;
    float getProgress() const; // 0.0 ~ 1.0
    int getProcessTime() const;
    int getEffectiveProcessTime() const;

    // 시나리오 변경용
    void setProcessTime(int t);
    void setProcessTimeMultiplier(float multiplier);

    void reset();

private:
    int m_processTime;
    int m_currentProgress = 0;
    bool m_hasItem = false;
    bool m_outputReady = false;
    float m_multiplier = 1.0f; // 시나리오에 따른 처리 시간 배수
};
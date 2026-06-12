#pragma once

class MachineHealth {
public:
    MachineHealth(float breakdownProb, int repairTime);

    bool rollBreakdown() const;
    void startRepair();
    bool tickRepair();

    void forceBreak();

    void applyBreakdownDamage(); // 고장 시 health 감소
    void applyRepairHeal();  // 수리 시 health 소량 회복

    float getHealth() const;
    float getBreakdownProb() const;
    int   getRepairTime() const;
    float getRepairProgress() const; // 0.0 ~ 1.0

    bool  wasForcedBreak() const;
    bool  isRetired() const; // health <= 0.0

    // forcedBreak 플래그 초기화 (PipelineEngine이 처리 후 호출)
    void clearForcedBreak();

    // 시나리오 변경용
    void setBreakdownProb(float prob);

    void reset();

private:
    float m_health = 1.0f;
    float m_breakdownProb;
    int m_repairTime;
    int m_repairProgress = 0;
    bool m_forcedBreak = false;

    static constexpr float BROKEN_SCALE = 0.1f;   // 고장 시 내구도 감소
    static constexpr float REPAIR_HP_SCALE = 0.05f;  // 수리 시 내구도 회복
};
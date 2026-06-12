#pragma once

class MachineStats {
public:
    void recordCompletion();
    void recordBreakdown();
    void incrementBrokenTicks();

    int getCompletedCount() const;
    int getBreakdownCount() const;
    int getBrokenTicks() const;

    // BROKEN_WAIT_TIME: 이 틱 수가 지나야 자동 수리 가능
    static constexpr int BROKEN_WAIT_TIME = 5;

    void resetBrokenTicks(); // 수리 후 brokenTicks만 초기화
    void reset();

private:
    int m_completedCount = 0;
    int m_breakdownCount = 0;
    int m_brokenTicks = 0;
};
// 파이프라인 로직과 무관하게 통계만 관리

class FactoryStatistics {
public:
    void recordFinished();
    void recordLost();
    void recordBreakdown();

    int getFinishedGoods() const;
    int getLostProducts() const;
    int getTotalBreakdowns() const;

    void reset();
private:
    int m_finishedGoods = 0;
    int m_lostProducts = 0;
    int m_totalBreakdowns = 0;
};
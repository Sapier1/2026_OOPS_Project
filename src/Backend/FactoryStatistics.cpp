#include "FactoryStatistics.h"

void FactoryStatistics::recordFinished() { ++m_finishedGoods; }
void FactoryStatistics::recordLost() { ++m_lostProducts; }
void FactoryStatistics::recordBreakdown() { ++m_totalBreakdowns; }

int FactoryStatistics::getFinishedGoods() const { return m_finishedGoods; }
int FactoryStatistics::getLostProducts() const { return m_lostProducts; }
int FactoryStatistics::getTotalBreakdowns() const { return m_totalBreakdowns; }

void FactoryStatistics::reset() {
    m_finishedGoods = 0;
    m_lostProducts = 0;
    m_totalBreakdowns = 0;
}
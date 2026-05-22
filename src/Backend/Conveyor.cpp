#include "Conveyor.h"

Conveyor::Conveyor(int capacity) : m_capacity(capacity) {}

bool Conveyor::push() {
    if (m_itemCount >= m_capacity) return false;
    ++m_itemCount;
    return true;
}

bool Conveyor::pop() {
    if (m_itemCount <= 0) return false;
    --m_itemCount;
    return true;
}

bool Conveyor::isEmpty()   const { return m_itemCount == 0; }
bool Conveyor::isFull()    const { return m_itemCount >= m_capacity; }
int  Conveyor::getSize()   const { return m_itemCount; }
int  Conveyor::getCapacity() const { return m_capacity; }

void Conveyor::reset() { m_itemCount = 0; }
void Conveyor::update(int /*tick*/) { /* 기본 컨베이어는 지연 없음 */ }
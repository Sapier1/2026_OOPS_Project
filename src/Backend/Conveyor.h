#pragma once
#include "SimulationObject.h"

// ─────────────────────────────────────────────
//  Conveyor  —  기계 사이 아이템 대기열
//
//  - push() : 아이템 추가. 포화 시 false 반환 (아이템 손실)
//  - pop()  : 아이템 꺼냄. 비어 있으면 false 반환
//  - update(): 현재 구현에서는 no-op (컨베이어 자체 지연 없음)
// ─────────────────────────────────────────────
class Conveyor : public SimulationObject {
private:
    int m_capacity;
    int m_itemCount = 0;

public:
    explicit Conveyor(int capacity = 5);

    bool push();           // 아이템 추가 → 성공 true, 포화 false
    bool pop();            // 아이템 꺼냄 → 성공 true, 비어있음 false
    bool isEmpty()  const;
    bool isFull()   const;
    int  getSize()  const;
    int  getCapacity() const;

    void reset();
    void update(int tick) override;  // no-op
};
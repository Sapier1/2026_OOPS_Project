#pragma once
#include <string>
using namespace std;

// UI 렌더링에 필요한 최소한의 데이터만 담는 구조체
struct MachineSnap {
    string name;
    int state;      // 0:IDLE, 1:WORKING, 2:BROKEN 3:REPAIRING
    float progress; // 0.0 ~ 1.0
    int queueSize; // 대기 중인 아이템 수
    int queueCapacity = 5; // 대기열 최대 수용량
    int completedCount; // 누적 완료 아이템 수
    float health; // 0.0 ~ 1.0, 고장 시 감소
    int processTime; // 총 작업 시간 (틱)
};
#pragma once
#include <string>
using namespace std;

// UI 렌더링에 필요한 최소한의 데이터만 담는 구조체
struct MachineSnap {
    string name;
    int state;      // 0:IDLE, 1:WORKING, 2:BROKEN 3:REPAIRING
    float progress; // 0.0 ~ 1.0
};
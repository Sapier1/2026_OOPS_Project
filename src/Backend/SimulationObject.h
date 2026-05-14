#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SimulationObject {
public:
    virtual ~SimulationObject() = default;
    
    // 매 프레임(tick)마다 호출될 핵심 로직,
    // 최상위 추상 클래스로서 모든 공장 객체(기계, 컨베이어)의 부모.
    // ImGui 관련 코드 기입 X.
    virtual void update(int tick) = 0;
};
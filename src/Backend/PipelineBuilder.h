#pragma once
#include <vector>
#include <memory>
#include "PipelineEngine.h"
#include "Conveyor.h"
#include "AbstractMachine.h"
using namespace std;

class PipelineBuilder {
public:
    void build(PipelineEngine& engine);

private:
    vector<unique_ptr<AbstractMachine>> m_machines;
    vector<unique_ptr<Conveyor>> m_conveyors;
};
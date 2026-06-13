#pragma once

#include <memory>

#include "SimulationCmd.h"
#include "IScenarioStrategy.h"

class ScenarioFactory
{
public:
    static std::unique_ptr<IScenarioStrategy>
    create(SimulationScenario scenario);
};
#include "ScenarioFactory.h"

#include "NormalFlowStrategy.h"
#include "RandomBreakdownStrategy.h"
#include "BottleNeckStrategy.h"
#include "OverFlowStrategy.h"

std::unique_ptr<IScenarioStrategy>
ScenarioFactory::create(SimulationScenario scenario)
{
    switch (scenario)
    {
    case SimulationScenario::NormalFlow:
        return std::make_unique<NormalFlowStrategy>();

    case SimulationScenario::RandomBreakdown:
        return std::make_unique<RandomBreakdownStrategy>();

    case SimulationScenario::BottleNeck:
        return std::make_unique<BottleNeckStrategy>();

    case SimulationScenario::OverFlow:
        return std::make_unique<OverFlowStrategy>();

    default:
        return nullptr;
    }
}
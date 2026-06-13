#pragma once

#include "IScenarioStrategy.h"

class OverFlowStrategy : public IScenarioStrategy
{
public:
    void apply(PipelineEngine& engine) override;
};
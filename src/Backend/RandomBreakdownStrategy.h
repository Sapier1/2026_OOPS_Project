#pragma once

#include "IScenarioStrategy.h"

class RandomBreakdownStrategy : public IScenarioStrategy
{
public:
    void apply(PipelineEngine& engine) override;
};
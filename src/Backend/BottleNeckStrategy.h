#pragma once

#include "IScenarioStrategy.h"

class BottleNeckStrategy : public IScenarioStrategy
{
public:
    void apply(PipelineEngine& engine) override;
};
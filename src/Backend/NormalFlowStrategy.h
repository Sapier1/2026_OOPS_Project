#pragma once

#include "IScenarioStrategy.h"

class NormalFlowStrategy : public IScenarioStrategy
{
public:
    void apply(PipelineEngine& engine) override;
};
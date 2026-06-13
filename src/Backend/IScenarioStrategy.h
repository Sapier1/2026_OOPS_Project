#pragma once

class PipelineEngine;

class IScenarioStrategy {
public:
    virtual ~IScenarioStrategy() = default;

    virtual void apply(PipelineEngine& engine) = 0;
};
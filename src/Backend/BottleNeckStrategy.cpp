#include "BottleNeckStrategy.h"

#include "PipelineEngine.h"

void BottleNeckStrategy::apply(PipelineEngine& engine)
{
    auto& nodes = engine.getNodes();

    for (auto& node : nodes)
    {
        node.machine->setBreakdownProb(0.02f);
        node.machine->setProcessTimeMultiplier(1.0f);
    }

    if (nodes.size() >= 2)
    {
        nodes[1].machine->setProcessTimeMultiplier(3.0f);
    }
}
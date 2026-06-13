#include "NormalFlowStrategy.h"

#include "PipelineEngine.h"

void NormalFlowStrategy::apply(PipelineEngine& engine)
{
    auto& nodes = engine.getNodes();

    for (auto& node : nodes)
    {
        node.machine->setBreakdownProb(0.02f);
        node.machine->setProcessTimeMultiplier(1.0f);
    }
}
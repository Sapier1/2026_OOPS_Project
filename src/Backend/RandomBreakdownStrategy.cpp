#include "RandomBreakdownStrategy.h"

#include "PipelineEngine.h"
#include "AbstractMachine.h"

void RandomBreakdownStrategy::apply(PipelineEngine& engine)
{
    auto& nodes = engine.getNodes();

    for (auto& node : nodes)
    {
        node.machine->setBreakdownProb(0.06f);
    }
}
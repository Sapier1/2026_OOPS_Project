#include "PipelineBuilder.h"
#include "MachineRegistry.h"

void PipelineBuilder::build(PipelineEngine& engine) {
    const auto& entries = MachineRegistry::instance().getAll();
    const int count = (int)entries.size();

    m_machines.reserve(count);
    for (const auto& entry : entries)
        m_machines.push_back(entry.create());

    // 컨베이어 수 = 기계 수 - 1
     m_conveyors.reserve(count - 1);
    for (int i = 0; i < count - 1; ++i)
        m_conveyors.push_back(make_unique<Conveyor>(5));

    // 노드 등록
    for (int i = 0; i < count; ++i) {
        Conveyor* in = (i == 0) ? nullptr : m_conveyors[i - 1].get();
        Conveyor* out = (i == count - 1) ? nullptr : m_conveyors[i].get();
        engine.registerNode(m_machines[i].get(), in, out);
    }
}
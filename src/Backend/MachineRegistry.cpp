#include "MachineRegistry.h"
#include <algorithm>

MachineRegistry& MachineRegistry::instance() {
    static MachineRegistry s;
    return s;
}

void MachineRegistry::registerMachine(function<unique_ptr<AbstractMachine>()> creator) {
    m_entries.push_back({creator});
}

const vector<MachineEntry>& MachineRegistry::getAll() {
    return m_entries;
}
#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "AbstractMachine.h"
using namespace std;

struct MachineEntry {
    function<unique_ptr<AbstractMachine>()> create;
};

class MachineRegistry {
public:
    static MachineRegistry& instance();

    void registerMachine(function<unique_ptr<AbstractMachine>()> creator);
    const vector<MachineEntry>& getAll();

private:
    vector<MachineEntry> m_entries;
    bool m_sorted = false;
};

#define REGISTER_MACHINE(ClassName) \
    static bool _registered_##ClassName = []() { \
        MachineRegistry::instance().registerMachine( \
            []() -> unique_ptr<AbstractMachine> { \
                return make_unique<ClassName>(); \
            } \
        ); \
        return true; \
    }();
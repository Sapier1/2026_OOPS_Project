#pragma once

#include "FactorySnap.h"

class FactorySimulation;
class MachineController;

class FactoryFloorView
{
public:
    void render(
        const FactorySnap& snap,
        FactorySimulation& factory,
        MachineController*& currentSelected
    );
};

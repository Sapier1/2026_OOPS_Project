#pragma once
#include "MachineController.h"

class IMachineControllerProvider {
public:
    virtual ~IMachineControllerProvider() = default;
    virtual size_t getMachineCount() const = 0;
    virtual MachineController& getMachineCtrl(size_t i) = 0;
};
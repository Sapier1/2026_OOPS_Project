#pragma once
#include "AbstractMachine.h"

class Cutter : public AbstractMachine {
public:
    Cutter();
    string getMachineName() const override;
};
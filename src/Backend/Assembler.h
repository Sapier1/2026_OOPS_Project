#pragma once
#include "AbstractMachine.h"

class Assembler : public AbstractMachine {
public:
    Assembler();
    string getMachineName() const override;
};
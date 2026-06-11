#pragma once
#include "AbstractMachine.h"

class Painter : public AbstractMachine {
public:
    Painter();
    string getMachineName() const override;
};
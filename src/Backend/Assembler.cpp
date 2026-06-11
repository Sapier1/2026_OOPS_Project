#include "Assembler.h"
#include <cstdlib>

REGISTER_MACHINE(Assembler)

// 5틱 작업, 2% 고장 확률, 5틱 수리 시간
Assembler::Assembler() : AbstractMachine(5, 0.05f, 5) {}

string Assembler::getMachineName() const { return "Assembler"; }
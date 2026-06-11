#include "Cutter.h"
#include <cstdlib>

REGISTER_MACHINE(Cutter)

// 3틱 작업, 2% 고장 확률, 4틱 수리 시간
Cutter::Cutter() : AbstractMachine(3, 0.02f, 4) {}

string Cutter::getMachineName() const { return "Cutter"; }
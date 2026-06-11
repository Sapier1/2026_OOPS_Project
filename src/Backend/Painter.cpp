#include "Painter.h"
#include <cstdlib>

REGISTER_MACHINE(Painter)

// 4틱 작업, 2% 고장 확률, 5틱 수리 시간
Painter::Painter() : AbstractMachine(4, 0.02f, 5) {}

string Painter::getMachineName() const { return "Painter"; }
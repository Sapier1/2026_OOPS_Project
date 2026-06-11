#pragma once
#include "MachineSnap.h"
#include "Backend/EventLogger.h"
#include <vector>

struct ConveyorSnap {
    int size     = 0;
    int capacity = 5;
};

struct FactorySnap {
    int  simState = 0;
    int  tick     = 0;
    int  scenario = 0;
    int  speed    = 1;

    int  finishedGoods   = 0;
    int  wipCount        = 0;
    int  totalBreakdowns = 0;
    int  lostProducts    = 0;

    vector<MachineSnap> machines;
    vector<ConveyorSnap> conveyors;
    vector<EventEntry> recentLogs;
};
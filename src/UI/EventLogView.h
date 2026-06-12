#pragma once

#include <vector>
#include "EventLogSnap.h"
#include "SimulationCmd.h"

class EventLogView {
public:
    void render(const std::vector<EventEntry>& logs, SimulationCmd& outCmd);
};
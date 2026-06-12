#pragma once

#include <vector>
#include "EventLogsnap.h"
#include "SimulationCmd.h"

class EventLogView {
public:
    void render(const std::vector<EventEntry>& logs, SimulationCmd& outCmd);
};
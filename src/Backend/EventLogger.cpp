#include "EventLogger.h"

void EventLogger::log(int tick, const string& msg) {
    m_log.push_back({ tick, msg });
    if ((int)m_log.size() > MAX_LOG)
        m_log.erase(m_log.begin());
}

const vector<EventEntry>& EventLogger::getLog() const {
    return m_log;
}

void EventLogger::clear() {
    m_log.clear();
}
#pragma once
#include <vector>
#include <string>
using namespace std;

struct EventEntry {
    int tick;
    string message;
};

class EventLogger {
public:
    void log(int tick, const string& msg);
    const vector<EventEntry>& getLog() const;
    void clear();
    
private:
    vector<EventEntry> m_log;
    static constexpr int MAX_LOG = 200;
};
#include "MachineProcessor.h"
#include <algorithm>

MachineProcessor::MachineProcessor(int processTime)
    : m_processTime(processTime)
{}

bool MachineProcessor::acceptItem() {
    if (m_hasItem) return false;
    m_hasItem         = true;
    m_currentProgress = 0;
    return true;
}

// 한 tick 진행. 완료 시 true.
bool MachineProcessor::tickWork() {
    if (!m_hasItem) return false;
    ++m_currentProgress;
    int effectiveTime = static_cast<int>(m_processTime * m_multiplier);
    if (m_currentProgress >= effectiveTime) {
        m_currentProgress = 0;
        m_hasItem = false;
        m_outputReady = true;
        return true;
    }
    return false;
}

void MachineProcessor::collectOutput() {
    m_outputReady = false;
}

bool MachineProcessor::hasItem() const { return m_hasItem; }
bool MachineProcessor::hasOutputReady() const { return m_outputReady; }
int MachineProcessor::getProcessTime() const { return m_processTime; }

float MachineProcessor::getProgress() const {
    int effectiveTime = static_cast<int>(m_processTime * m_multiplier);
    if (effectiveTime <= 0) return 0.0f;
    return static_cast<float>(m_currentProgress) / effectiveTime;
}

int MachineProcessor::getEffectiveProcessTime() const {
    return static_cast<int>(m_processTime * m_multiplier);
}

void MachineProcessor::setProcessTime(int t) {
    m_processTime = t;
}

void MachineProcessor::setProcessTimeMultiplier(float multiplier) {
    m_multiplier = (multiplier > 0.0f) ? multiplier : 1.0f;
}

void MachineProcessor::reset() {
    m_currentProgress = 0;
    m_hasItem = false;
    m_outputReady = false;
}
#include "second_thread.h"

#include "satellite/satellite.h"

WorkerManager::WorkerManager() : m_running(false) {
    std::lock_guard<std::mutex> lock(m_time_mutex);
    m_timestamp = libsgp4::DateTime::Now();
}

WorkerManager::~WorkerManager() {
    stop();
}

void WorkerManager::start() {
    if (!m_running) {
        {
            std::lock_guard<std::mutex> lock(m_time_mutex);
            m_timestamp = libsgp4::DateTime::Now();
        }
        m_running = true;
        m_worker = std::thread(&WorkerManager::WorkerLoop, this);
    }
}

void WorkerManager::stop() {
    if (m_running) {
        m_running = false;
        if (m_worker.joinable()) {
            m_worker.join();
        }
    }
}

void WorkerManager::WorkerLoop() {
    while (m_running) {
        {
            std::lock_guard<std::mutex> lock(m_time_mutex);
            m_timestamp = m_timestamp.AddSeconds(time_multiplier);
        }
        {
            std::lock_guard<std::mutex> lock(get_satellite_mutex());
            for (auto& satellite : get_all_satellites()) {
                satellite.update(m_timestamp);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(update_interval_ms));
    }
}

libsgp4::DateTime WorkerManager::get_current_time() {
    std::lock_guard<std::mutex> lock(m_time_mutex);
    return m_timestamp;
}
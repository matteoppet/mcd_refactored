#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

#include "libsgp4/DateTime.h"

class WorkerManager {
    private:
        std::thread m_worker;
        std::atomic<bool> m_running;

        libsgp4::DateTime m_timestamp;
        std::mutex m_time_mutex;

        void WorkerLoop();

    public:
        float time_multiplier = 1.0f;
        int update_interval_ms = 10;

        WorkerManager();
        ~WorkerManager();

        void start();
        void stop();
        libsgp4::DateTime get_current_time();

        bool IsRunning() const { return m_running; }
};
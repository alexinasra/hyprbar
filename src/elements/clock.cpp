#include "./clock.hpp"
#include <chrono>
#include <iomanip>
#include <print>
#include <sstream> 
#include <thread>
#include <ctime>
#include <string>


Clock::Clock() {
    clockLabel = Hyprtoolkit::CTextBuilder::begin()
        ->commence();
    std::thread tupdateThread([&] {
        while (!stopTimeUpdate) {
            // Get the current time point using std::chrono::system_clock
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            // Convert to local time structure (thread-safe in C++20, but use std::mutex for general compatibility)
            std::tm localTime = {};
            localtime_r(&currentTime, &localTime); // POSIX version

            std::ostringstream timeStream;
            timeStream << std::put_time(&localTime, format.c_str());
            std::string timeStr = timeStream.str();

            // Lock the mutex before writing to the console
            std::lock_guard<std::mutex> lock(timeUpdateMutex);
            // std::cout << timeStr;
            // std::cout.flush();
            clockLabel->rebuild()->text(std::string(timeStr))->commence();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::println("thread ended");
    });
    tupdateThread.detach();
 
};


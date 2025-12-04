#include "./Clock.hpp"
#include <chrono>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/types/SizeType.hpp>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <sstream> 
#include <thread>
#include <ctime>
#include <string>


hyprbar::Clock::Clock() {
    layout = CColumnLayoutBuilder::begin()->commence();
    clockLabel = Hyprtoolkit::CButtonBuilder::begin()
        ->commence();
    layout->addChild(clockLabel);
    
    std::thread tupdateThread([&] {
        while (!stopTimeUpdate) {
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
            std::tm localTime = {};
            localtime_r(&currentTime, &localTime);
            std::ostringstream timeStream;
            timeStream << std::put_time(&localTime, format.c_str());
            std::string timeStr = timeStream.str();
            std::lock_guard<std::mutex> lock(timeUpdateMutex);
            clockLabel->rebuild()->label(std::string(timeStr)) 
                ->commence();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    tupdateThread.detach(); 
};


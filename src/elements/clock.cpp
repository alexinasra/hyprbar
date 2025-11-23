#include "./clock.hpp"
#include <chrono>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/types/SizeType.hpp>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <sstream> 
#include <thread>
#include <ctime>
#include <string>


Clock::Clock() {
     clockLabel = Hyprtoolkit::CButtonBuilder::begin()
                ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {80, 30}})
                ->commence();
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
            clockLabel->rebuild()
                ->onMainClick([&](auto) {
                        spdlog::info("todo: Open Calendar widget");
                })
                ->label(std::string(timeStr)) 
                ->commence();
            clockLabel->setTooltip("SomeText");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    tupdateThread.detach(); 
};


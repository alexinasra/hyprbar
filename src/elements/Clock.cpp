#include "./Clock.hpp"
#include "src/config/ConfigManager.hpp"
#include <chrono>
#include <hyprlang.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/types/SizeType.hpp>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <sstream> 
#include <thread>
#include <ctime>
#include <string>


hyprbar::Clock::Clock() {
    static auto CLOCK_FORMAT = Hyprlang::CSimpleConfigValue<Hyprlang::STRING>(
                g_configManager->m_config.get(), "clock:format");
    layout = CColumnLayoutBuilder::begin()->commence();
    clockLabel = Hyprtoolkit::CTextBuilder::begin()
        ->commence();
    layout->addChild(clockLabel);
    
    std::thread tupdateThread([&] {
        while (!stopTimeUpdate) {
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
            std::tm localTime = {};
            localtime_r(&currentTime, &localTime);
            std::ostringstream timeStream;
            timeStream << std::put_time(&localTime, *CLOCK_FORMAT);
            std::string timeStr = timeStream.str();
            std::lock_guard<std::mutex> lock(timeUpdateMutex);
            clockLabel->rebuild()->text(std::string(timeStr)) 
                ->commence();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    tupdateThread.detach(); 
};

hyprbar::Clock* hyprbar::ClockBuilder::create() {
    return new Clock();
}

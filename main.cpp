#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/core/CoreMacros.hpp>

#include <hyprtoolkit/window/Window.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/element/Image.hpp>
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/Null.hpp>

#include <hyprland/protocols/wlr-layer-shell-unstable-v1.hpp>
#include <hyprland/protocols/wayland.hpp>
#include <hyprland/protocols/xdg-shell.hpp>
#include <hyprutils/memory/Atomic.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/memory/UniquePtr.hpp>
#include <hyprutils/string/ConstVarList.hpp>
#include <hyprutils/string/String.hpp>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <print>
#include <sstream> 
#include <thread>
#include <ctime>
#include <atomic>
#include <mutex>


using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;
using namespace Hyprutils::String;
using namespace Hyprtoolkit;

#define SP CSharedPointer
#define WP CWeakPointer
#define UP CUniquePointer

static SP<IBackend> backend;

std::atomic<bool> stopTimeUpdateThread(false);
std::mutex timeUpdateMutex;

int main(int argc, char** argv, char** envp) {
    //setenv("HT_QUIET", "1", true);
    backend = IBackend::create();


    uint32_t anchor_flags = ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP; 

    auto window =
        CWindowBuilder::begin()
            ->appTitle("Hyprpanel")
            ->appClass("hypr-panel")
            ->marginTopLeft({10,0})
            ->maxSize({5000,30})
            ->minSize({100,30})
            ->preferredSize({1920,30})
            ->type(Hyprtoolkit::HT_WINDOW_LAYER)
            ->layer(ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM)
            ->anchor(XDG_POSITIONER_ANCHOR_TOP)
            ->exclusiveZone(30)
            ->exclusiveEdge(XDG_TOPLEVEL_RESIZE_EDGE_NONE)
            ->commence();
   // window->m_rootElement->addChild(CRectangleBuilder::begin()->color([] { return backend->getPalette()->m_colors.background; })->commence());

    auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})->commence();
    layout->setMargin(0);

    auto layoutInner = CRowLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1.F, 1.F}})->commence();

    window->m_rootElement->addChild(layout);

    layout->addChild(layoutInner);
    layoutInner->setGrow(true);
    layoutInner->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    layoutInner->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);

    auto button = CButtonBuilder::begin()->label("My Label")->onMainClick([](auto){
            std::println("{}", std::chrono::system_clock::now());
            })->commence();
    
    auto time_label = CTextBuilder::begin()
        ->commence();

    std::thread tupdateThread([
        lbl = Hyprutils::Memory::CSharedPointer<CTextElement>{time_label}
    ] {
        while (!stopTimeUpdateThread) {
            // Get the current time point using std::chrono::system_clock
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            // Convert to local time structure (thread-safe in C++20, but use std::mutex for general compatibility)
            std::tm localTime = {};
            localtime_r(&currentTime, &localTime); // POSIX version

            // Format the time string
            std::ostringstream timeStream;
             timeStream << std::put_time(&localTime, "%H:%M:%S");
            std::string timeStr = timeStream.str();

            // Lock the mutex before writing to the console
            std::lock_guard<std::mutex> lock(timeUpdateMutex);
            // std::cout << timeStr;
            // std::cout.flush();
            lbl->rebuild()->text(std::string{timeStr})->commence();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::println("thread ended");
    });
    tupdateThread.detach();
    layoutInner->addChild(button);
    layoutInner->addChild(time_label);

    window->m_events.closeRequest.listenStatic([w = WP<IWindow>{window}] {
        stopTimeUpdateThread = true;
        w->close();
        backend->destroy();    
    });
   
    std::cout << "openning\n"; 
    window->open();
    backend->enterLoop();

    return 0;
}

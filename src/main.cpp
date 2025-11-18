#include "src/IPC.hpp"
#include "src/elements/clock.hpp"
#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/core/CoreMacros.hpp>

#include <hyprtoolkit/types/SizeType.hpp>
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
#include <chrono>
#include <print>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h" 
#include "src/elements/workspaces.hpp"

using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;
using namespace Hyprutils::String;
using namespace Hyprtoolkit;

#define SP CSharedPointer
#define WP CWeakPointer
#define UP CUniquePointer

static SP<IBackend> backend;


int main(int argc, char** argv, char** envp) {
    //setenv("HT_QUIET", "1", true);
    auto default_logger = spdlog::stdout_color_st("HBAR");
    spdlog::set_default_logger(default_logger);

    backend = IBackend::create();

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
  
    auto spacer = CNullBuilder::begin()->commence();
    spacer->setGrow(true);
   
    Clock c; 
    c.getLabel()->setPositionFlag(IElement::HT_POSITION_FLAG_RIGHT,true);
    Workspaces workspaces; 
    auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})->commence();
    layout->setMargin(0);

    auto layoutInner = CRowLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1.F, 1.F}})->commence();
    
    auto leftLayout = CRowLayoutBuilder::begin()->commence();
    auto leftLayoutInner = CRowLayoutBuilder::begin()->commence();
    
    auto centerLayout = CRowLayoutBuilder::begin()->commence();
    centerLayout->setGrow(true);
    auto centerLayoutInner = CRowLayoutBuilder::begin()->commence();


    auto rightLayout = CRowLayoutBuilder::begin()->commence();
    auto rightLayoutInner = CRowLayoutBuilder::begin()->commence();
    

    window->m_rootElement->addChild(layout);

    layout->addChild(layoutInner);
    layoutInner->addChild(leftLayout);
    layoutInner->addChild(centerLayout);
    layoutInner->addChild(rightLayout); 
    layoutInner->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    layoutInner->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);
    
    leftLayout->addChild(leftLayoutInner);
    centerLayout->addChild(centerLayoutInner);
    rightLayout->addChild(rightLayoutInner);

    leftLayoutInner->addChild(workspaces.getWorkspacesLayout());
    rightLayoutInner->addChild(spacer);
    rightLayoutInner->addChild(c.getLabel());
    window->m_events.closeRequest.listenStatic([w = WP<IWindow>{window}] {
        w->close();
        backend->destroy();    
    });
   
    window->open();
    backend->enterLoop();

    return 0;
}

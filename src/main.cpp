#include "src/Hyprbar.hpp"
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
#include <spdlog/spdlog.h>

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
    backend = IBackend::create();
    hyprbar::Hyprbar hbar;
    CSharedPointer<IWindow> window = hbar.getWindow();

    window->m_events.closeRequest.listenStatic([w = WP<IWindow>{window}] {
        w->close();
        backend->destroy();    
    });
   
    window->open();
    backend->enterLoop();

    return 0;
}

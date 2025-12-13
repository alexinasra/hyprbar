#pragma once

#include <hyprland/protocols/wlr-layer-shell-unstable-v1.hpp>
#include <hyprland/protocols/wayland.hpp>
#include <hyprland/protocols/xdg-shell.hpp>

#include <hyprtoolkit/core/CoreMacros.hpp>
#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/window/Window.hpp>
#include "./helpers/Memory.hpp"

using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;
using namespace Hyprtoolkit;

namespace hyprbar {

class Hyprbar {
public:
    Hyprbar();
    virtual ~Hyprbar(){};
    inline SP<IWindow> getWindow() { return window; }; 
    void show();
    void hide();

    void addToLeftLayout(SP<IElement> widget);
    void addToCenterLayout(SP<IElement> widget);
    void addToRightLayout(SP<IElement> widget);
private:
    SP<IWindow> window;
    SP<CRowLayoutElement> containerLayout;
    SP<CRowLayoutElement> leftLayout;
    SP<CRowLayoutElement> centerLayout;
    SP<CRowLayoutElement> rightLayout;
    int height = 30;
};
}



#pragma once

#include <hyprland/protocols/wlr-layer-shell-unstable-v1.hpp>
#include <hyprland/protocols/wayland.hpp>
#include <hyprland/protocols/xdg-shell.hpp>

#include <hyprtoolkit/core/CoreMacros.hpp>
#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/window/Window.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;
using namespace Hyprtoolkit;

namespace hyprbar {

class Hyprbar {
public:
    Hyprbar();
    virtual ~Hyprbar(){};
    inline CSharedPointer<IWindow> getWindow() { return window; }; 
    void show();
    void hide();

    void addToLeftLayout(CSharedPointer<IElement> widget);
    void addToCenterLayout(CSharedPointer<IElement> widget);
    void addToRightLayout(CSharedPointer<IElement> widget);
private:
    CSharedPointer<IWindow> window;
    CSharedPointer<CRowLayoutElement> containerLayout;
    CSharedPointer<CRowLayoutElement> leftLayout;
    CSharedPointer<CRowLayoutElement> centerLayout;
    CSharedPointer<CRowLayoutElement> rightLayout;
    int height = 30;
};
}



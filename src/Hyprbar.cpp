#include "Hyprbar.hpp"
#include "src/Modules.hpp"
#include "src/config/ConfigManager.hpp"
#include <hyprlang.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprutils/memory/UniquePtr.hpp>



hyprbar::Hyprbar::Hyprbar () {
    static auto BAR_HEIGHT  = Hyprlang::CSimpleConfigValue<Hyprlang::INT>(g_configManager->m_config.get(), "height");
    window = Hyprtoolkit::CWindowBuilder::begin()
        ->appTitle("Hyprbar")
        ->appClass("Hyprbar")
        ->marginTopLeft({0,0})
        ->preferredSize({1920, (int) *BAR_HEIGHT})
        ->type(Hyprtoolkit::HT_WINDOW_LAYER)
        ->layer(ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM)
        ->anchor(XDG_POSITIONER_ANCHOR_TOP)
        ->exclusiveZone(*BAR_HEIGHT)
        ->exclusiveEdge(XDG_TOPLEVEL_RESIZE_EDGE_NONE)
        ->commence();
 
    // Left side layout
    leftLayout = CRowLayoutBuilder::begin()
        ->commence();
    leftLayout->addChild(Modules::inst().getModule("Workspaces")->getWidget());

    //Center layout
    centerLayout = CRowLayoutBuilder::begin()
        ->commence();
    centerLayout->setGrow(true);
    centerLayout->addChild(Modules::inst().getModule("ActiveWindow")->getWidget());

    //Right side layout
    rightLayout = CRowLayoutBuilder::begin()
        ->commence();
    rightLayout->addChild(Modules::inst().getModule("Battery")->getWidget());
    rightLayout->addChild(Modules::inst().getModule("Clock")->getWidget()); 
    
    // main container layout
    containerLayout = CRowLayoutBuilder::begin()
        ->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}}) 
        ->commence();    

    containerLayout->setGrow(true);
    containerLayout->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    containerLayout->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);

    containerLayout->addChild(leftLayout);
    containerLayout->addChild(centerLayout);
    containerLayout->addChild(rightLayout);
    
    window->m_rootElement->addChild(containerLayout);
}

void hyprbar::Hyprbar::addToLeftLayout(SP<IElement> widget) {
    leftLayout->addChild(widget);
}

void hyprbar::Hyprbar::addToCenterLayout(SP<IElement> widget) {
    centerLayout->addChild(widget);
}

void hyprbar::Hyprbar::addToRightLayout(SP<IElement> widget) {
    rightLayout->addChild(widget);
}

void hyprbar::Hyprbar::show() {
    window->open();
}

void hyprbar::Hyprbar::hide() {
    window->close();
}


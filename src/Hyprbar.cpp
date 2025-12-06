#include "Hyprbar.hpp"
#include <hyprtoolkit/element/Rectangle.hpp>




hyprbar::Hyprbar::Hyprbar () {
    window = Hyprtoolkit::CWindowBuilder::begin()
        ->appTitle("Hyprpanel")
        ->appClass("hypr-panel")
        ->marginTopLeft({0,0})
        ->maxSize({5000,height})
        ->minSize({100,height})
        ->preferredSize({1920,height})
        ->type(Hyprtoolkit::HT_WINDOW_LAYER)
        ->layer(ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM)
        ->anchor(XDG_POSITIONER_ANCHOR_TOP)
        ->exclusiveZone(height)
        ->exclusiveEdge(XDG_TOPLEVEL_RESIZE_EDGE_NONE)
        ->commence();
   
    // Left side layout
    leftLayout = CRowLayoutBuilder::begin()
        ->commence();
    leftLayout->addChild(workspaces.getWidget());

    //Center layout
    centerLayout = CRowLayoutBuilder::begin()
        ->commence();
    centerLayout->setGrow(true);
    centerLayout->addChild(activeWindow.getWidget());

    //Right side layout
    rightLayout = CRowLayoutBuilder::begin()
        ->commence();
    rightLayout->addChild(battery.getWidget());
    rightLayout->addChild(clock.getWidget()); 
    
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

void hyprbar::Hyprbar::addToLeftLayout(CSharedPointer<IElement> widget) {
    leftLayout->addChild(widget);
}

void hyprbar::Hyprbar::addToCenterLayout(CSharedPointer<IElement> widget) {
    centerLayout->addChild(widget);
}

void hyprbar::Hyprbar::addToRightLayout(CSharedPointer<IElement> widget) {
    rightLayout->addChild(widget);
}

void hyprbar::Hyprbar::show() {
    window->open();
}

void hyprbar::Hyprbar::hide() {
    window->close();
}


#pragma once

#include <hyprtoolkit/element/RowLayout.hpp>
#include <libudev.h>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <unistd.h> // For read()
#include <hyprtoolkit/element/Element.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;

namespace hyprbar {

class Battery {
public:
    Battery();
    ~Battery();
    inline CSharedPointer<IElement> getWidget(){ return layout; };
private:
    struct udev *udev;
    struct udev_monitor *mon;
    CSharedPointer<IElement> layout;
};

}

#pragma once

#include "src/elements/IModule.hpp"
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

class Battery : public IModule {
public:
    Battery();
    ~Battery() override;
    CSharedPointer<IElement> getWidget() override { return layout; };
private:
    struct udev *udev;
    struct udev_monitor *mon;
    CSharedPointer<IElement> layout;
};

}

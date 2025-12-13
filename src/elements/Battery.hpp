#pragma once

#include "src/elements/IModule.hpp"
#include <hyprtoolkit/element/RowLayout.hpp>
#include <libudev.h>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <unistd.h> // For read()
#include <hyprtoolkit/element/Element.hpp>
#include "../helpers/Memory.hpp"

using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;

namespace hyprbar {

class Battery : public IModule {
public:
    Battery();
    ~Battery() override;
    SP<IElement> getWidget() override { return layout; };
private:
    struct udev *udev;
    struct udev_monitor *mon;
    SP<IElement> layout;
    std::string create_label_text(std::string c, std::string s);
};


class BatteryBuilder : public IModuleBuilder {
public:
    BatteryBuilder() {};
    ~BatteryBuilder() override {};
    IModule* create() override;
};
}

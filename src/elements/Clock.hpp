#pragma once


#include "src/elements/IModule.hpp"
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Element.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include "../helpers/Memory.hpp"
#include <atomic>
#include <mutex>

using namespace Hyprutils::Memory;
using namespace Hyprtoolkit;


namespace hyprbar{
class Clock : public hyprbar::IModule {
public:
    Clock();
    SP<IElement> getWidget() override { return layout; }
private:
    SP<CTextElement> clockLabel;
    SP<CColumnLayoutElement> layout;
    std::mutex timeUpdateMutex;
    std::atomic<bool> stopTimeUpdate;

};

class ClockBuilder : public IModuleBuilder {
public:
    ClockBuilder(){};
    ~ClockBuilder() override {};
    Clock* create() override;
};
}

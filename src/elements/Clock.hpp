#pragma once


#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Element.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <atomic>
#include <mutex>

using namespace Hyprutils::Memory;
using namespace Hyprtoolkit;


namespace hyprbar {
class Clock {
public:
    Clock();
    inline void setFormat(std::string f){ format=f; };
    inline CSharedPointer<IElement> getWidget() { return layout; }
private:
    std::string format = "%c";
    CSharedPointer<CButtonElement> clockLabel;
    CSharedPointer<CColumnLayoutElement> layout;
    std::mutex timeUpdateMutex;
    std::atomic<bool> stopTimeUpdate;

};

}

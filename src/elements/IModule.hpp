#pragma once


#include <hyprtoolkit/element/Element.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
namespace hyprbar {

class IModule {
public:
    virtual ~IModule() {}
    virtual Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IElement> getWidget() = 0;
};
}

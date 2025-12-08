#pragma once 

#include "src/IPC.hpp"
#include "src/elements/IModule.hpp"
#include <hyprtoolkit/element/Element.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprutils/memory/SharedPtr.hpp>



using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;


namespace hyprbar {

class ActiveWindow : public IModule, EventHandler {
public:
    ActiveWindow();
    ~ActiveWindow() override;
    CSharedPointer<IElement> getWidget() override { return layout; };
private:
    CSharedPointer<CRowLayoutElement> layout;
    CSharedPointer<CTextElement> titleLabel;
    IPC& ipc = IPC::inst();
    std::mutex ipc_mutex;
    void onEvent(const std::string& e) override;
    void rebuild();
};

}



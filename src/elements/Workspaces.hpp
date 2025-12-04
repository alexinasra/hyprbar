#pragma once


#include "src/IPC.hpp"
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <mutex>

using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;

namespace hyprbar {
class Workspaces : public EventHandler
{
public:
    Workspaces();
    inline CSharedPointer<CRowLayoutElement> getWidget() { return workspacesLayout; };

private:
    IPC& ipc = IPC::inst();
    CSharedPointer<CRowLayoutElement> workspacesLayout;
    std::mutex ipc_mutex;
    void onEvent(const std::string& e) override;
    void rebuild();

};

}

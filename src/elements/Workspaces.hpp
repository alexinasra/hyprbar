#pragma once


#include "src/IPC.hpp"
#include "src/elements/IModule.hpp"
#include <hyprtoolkit/element/Element.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <mutex>
#include "../helpers/Memory.hpp"

using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;

namespace hyprbar {
class Workspaces : public IModule, EventHandler {
public:
    Workspaces();
    SP<IElement> getWidget() override { return workspacesLayout; };

private:
    IPC& ipc = IPC::inst();
    SP<CRowLayoutElement> workspacesLayout;
    std::mutex ipc_mutex;
    void onEvent(const std::string& e) override;
    void rebuild();

};


class WorkspacesBuilder: public IModuleBuilder {
public:
    WorkspacesBuilder(){};
    ~WorkspacesBuilder() override {};
    Workspaces* create() override;
};
}

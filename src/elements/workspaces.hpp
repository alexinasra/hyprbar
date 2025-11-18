#pragma once


#include "src/IPC.hpp"
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <mutex>


class Workspaces : public hyprbar::EventHandler
{
    public:
        Workspaces();
        inline Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRowLayoutElement> getWorkspacesLayout() { return workspacesLayout; };



    private:
        hyprbar::IPC& ipc = hyprbar::IPC::inst();
        Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRowLayoutElement> workspacesLayout;
        std::mutex ipc_mutex;
        void onEvent(const std::string& e) override;
        void rebuild();

};

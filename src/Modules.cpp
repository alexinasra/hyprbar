#include "Modules.hpp"
#include "src/elements/ActiveWindow.hpp"
#include <hyprutils/memory/SharedPtr.hpp>



hyprbar::Modules& hyprbar::Modules::inst() {
    static Modules m;
    return m;
}

hyprbar::Modules::Modules() {
    m_map["ActiveWindow"] = std::make_unique<ActiveWindowBuilder>();
    m_map["Battery"] = std::make_unique<BatteryBuilder>();
    m_map["Clock"] = std::make_unique<ClockBuilder>();
    m_map["Workspaces"] = std::make_unique<WorkspacesBuilder>();
}

hyprbar::IModule* hyprbar::Modules::getModule(std::string n) {
    return  m_map.at(n).get()->create();
}



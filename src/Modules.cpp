#include "Modules.hpp"
#include <hyprutils/memory/SharedPtr.hpp>



hyprbar::Modules& hyprbar::Modules::inst() {
    static Modules m;
    return m;
}

hyprbar::Modules::Modules() {
    m_map["ActiveWindow"] = std::unique_ptr<ActiveWindow>(new ActiveWindow());
    m_map["Battery"] = std::unique_ptr<Battery>(new Battery());
    m_map["Clock"] = std::unique_ptr<Clock>(new Clock());
    m_map["Workspaces"] = std::unique_ptr<Workspaces>(new Workspaces());
}

hyprbar::IModule* hyprbar::Modules::getModule(std::string n) {
    auto m = m_map.at(n).get();    
    return m;
}


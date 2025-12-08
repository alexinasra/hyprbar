#pragma once 

#include <memory>
#include <unordered_map>



#include "src/elements/ActiveWindow.hpp"
#include "src/elements/Battery.hpp"
#include "src/elements/Clock.hpp"
#include "src/elements/IModule.hpp"
#include "src/elements/Workspaces.hpp"
namespace hyprbar {


class Modules {
public:
    static Modules& inst();
    IModule* getModule(std::string n);
    //void addModule(const std::string n, IModule m);
    //void removeModule(const std::string n);
private:
    Modules();
    std::unordered_map<std::string, std::unique_ptr<IModule>> m_map;    
};
}


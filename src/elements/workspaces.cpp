#include "workspaces.hpp"
#include <algorithm>
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/palette/Color.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <json/value.h>
#include <spdlog/spdlog.h>
#include <vector>


void sortJsonArray(Json::Value& jsonArray) {
    // 1. Copy the Json::Value elements to a std::vector
    std::vector<Json::Value> values;
    for (const auto& value : jsonArray) {
        values.push_back(value);
    }

    // 2. Use std::sort with a lambda comparator
    std::sort(values.begin(), values.end(), [&](const Json::Value& a, const Json::Value& b) {
        // Access the values to compare by key
        // Ensure the values are of the correct type (e.g., string) before comparison
        return a["id"].asInt() < b["id"].asInt();
    });

    // 3. Clear the original Json::Value and repopulate it in the new order
    jsonArray.clear();
    for (const auto& value : values) {
        jsonArray.append(value);
    }
}


Workspaces::Workspaces() {
    workspacesLayout = Hyprtoolkit::CRowLayoutBuilder::begin()->commence();
    
    rebuild();
    
    ipc.registerForIPC("workspacev2", this);
    ipc.registerForIPC("activespecial", this);
    ipc.registerForIPC("createworkspacev2", this);
    ipc.registerForIPC("destroyworkspacev2", this);
    ipc.registerForIPC("focusedmonv2", this);
    ipc.registerForIPC("moveworkspacev2", this);
    ipc.registerForIPC("renameworkspace", this);
    ipc.registerForIPC("openwindow", this);
    ipc.registerForIPC("closewindow", this);
    ipc.registerForIPC("movewindowv2", this);
    ipc.registerForIPC("urgent", this);
    ipc.registerForIPC("configreloaded", this);
}

void Workspaces::onEvent(const std::string &ev) {

    std::lock_guard<std::mutex> lock(ipc_mutex);
    spdlog::info("Incoming Event");
    spdlog::info(ev);
    rebuild();
}

void Workspaces::rebuild() {
    workspacesLayout->clearChildren();
    auto reply = ipc.getSocket1JsonReply("activeworkspace");
    auto activeId = reply["id"].asInt();
    reply = ipc.getSocket1JsonReply("workspaces");
    sortJsonArray(reply);
    for (auto const &workspace : reply) {
        auto id =  workspace["id"].asInt();
        std::string lbl = workspace["id"].asString();
        if(workspace["name"].asString() != workspace["id"].asString()) {
            lbl.append(" : " + workspace["name"].asString());
        }
        std::string dispatchChange = "dispatch workspace " + workspace["id"].asString();
        auto cb = [dispatchChange = dispatchChange, this](auto){
            ipc.getSocket1Reply(dispatchChange);       
        };
        auto btn = Hyprtoolkit::CButtonBuilder::begin()
            ->label(lbl.c_str())
            ->onMainClick(cb)
            ->noBg(activeId != id)
            ->commence();
        
        if (activeId == id) {
            auto bg = Hyprtoolkit::CRectangleBuilder::begin()
                ->color([](){ return Hyprtoolkit::CHyprColor{1.F, 1.F, 1.F, 0.1F}; })
                ->commence();
            btn->addChild(bg);

        }
        workspacesLayout->addChild(btn);
    }



}

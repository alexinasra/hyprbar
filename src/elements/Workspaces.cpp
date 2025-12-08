#include "Workspaces.hpp"
#include <algorithm>
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/palette/Color.hpp>
#include <hyprtoolkit/types/SizeType.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <json/value.h>
#include <spdlog/spdlog.h>
#include <vector>
#include <chrono>

void sortJsonArray(Json::Value& jsonArray) {
    std::vector<Json::Value> values;
    for (const auto& value : jsonArray) {
        values.push_back(value);
    }

    std::sort(values.begin(), values.end(), [&](const Json::Value& a, const Json::Value& b) {
        return a["id"].asInt() < b["id"].asInt();
    });

    jsonArray.clear();
    for (const auto& value : values) {
        jsonArray.append(value);
    }
}


hyprbar::Workspaces::Workspaces() {
    workspacesLayout = CRowLayoutBuilder::begin()
        ->gap(4)
        ->commence();
    
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

void hyprbar::Workspaces::onEvent(const std::string &ev) {

    std::lock_guard<std::mutex> lock(ipc_mutex);
    spdlog::info("Incoming Event");
    spdlog::info(ev);
    rebuild();
}

void hyprbar::Workspaces::rebuild() {
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
            ->size({CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, { 40, 30 }})
            ->commence();
        if (activeId == id) {
            auto bg = CRectangleBuilder::begin()
                ->color([](){ return CHyprColor{1.F, 1.F, 1.F, 0.1F}; })
                ->commence();
            bg->setGrow(true);
            btn->addChild(bg);

        }
        workspacesLayout->addChild(btn);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}


hyprbar::Workspaces * hyprbar::WorkspacesBuilder::create(){
    return new Workspaces();
}


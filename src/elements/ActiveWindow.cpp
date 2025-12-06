#include "ActiveWindow.hpp"
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/Text.hpp>



hyprbar::ActiveWindow::ActiveWindow() {
    layout = CRowLayoutBuilder::begin()
        ->commence();
    titleLabel = CTextBuilder::begin()->commence(); 
    layout->addChild(titleLabel);
    rebuild();
    ipc.registerForIPC("activewindow", this);
}

void hyprbar::ActiveWindow::onEvent(const std::string &ev) {
    std::lock_guard<std::mutex> lock(ipc_mutex);
    rebuild();
}




hyprbar::ActiveWindow::~ActiveWindow() {}


void hyprbar::ActiveWindow::rebuild() {
    auto reply = ipc.getSocket1JsonReply("activewindow");
    std::string wtitle = reply["title"].asString();
    titleLabel->rebuild()
        ->text(wtitle.c_str())
        ->commence();
} 

#pragma once


#include <hyprtoolkit/element/Text.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <atomic>
#include <mutex>


class Clock {
    public:
        Clock();
        Clock(Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CTextElement> clockLabel);
        inline Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CTextElement> getLabel() { return clockLabel; }; 
    private:
        std::string format = "%H:%M";
        Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CTextElement> clockLabel;
        std::mutex timeUpdateMutex;
        std::atomic<bool> stopTimeUpdate;

};

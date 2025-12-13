#include  "Battery.hpp"
#include <hyprlang.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <libudev.h>
#include <mutex>
#include <spdlog/spdlog.h>
#include <sstream>
#include <thread>


std::mutex mtx;
hyprbar::Battery::~Battery() {
    /* Cleanup (unreachable in this infinite loop, but good practice for finite programs) */
    //udev_monitor_unref(mon);
    udev_unref(udev);
}

hyprbar::Battery::Battery() {
    layout = CRowLayoutBuilder::begin()
        ->gap(2)
        ->commence();
    auto batterylabel = CTextBuilder::begin()
       ->text("NO BATTERY")
       ->fontFamily(Hyprlang::STRING{"FontAwesome"})
       ->commence(); 

    layout->addChild(batterylabel);

    /* Create the udev library context */
    udev = udev_new();
    if (!udev) {
        spdlog::error("Cannot create udev context.\n");
        exit(EXIT_FAILURE);
    }

    std::thread th([batterylabel, this]() {
        /* Get the file descriptor for the monitor */
        while(true) {
            struct udev_enumerate *enumerate;
            struct udev_list_entry *devices, *dev_list_entry;

            enumerate = udev_enumerate_new(udev);
            udev_enumerate_add_match_subsystem(enumerate, "power_supply");
            udev_enumerate_scan_devices(enumerate);
            devices = udev_enumerate_get_list_entry(enumerate);
            udev_list_entry_foreach(dev_list_entry, devices) {
            
            const char *path;
            struct udev_device *dev;
            path = udev_list_entry_get_name(dev_list_entry);
            dev = udev_device_new_from_syspath(udev, path);

            if (dev) {
                const char *type = udev_device_get_property_value(dev, "POWER_SUPPLY_TYPE");
                if (type && strcmp(type, "Battery") == 0) {
                    std::string c = udev_device_get_sysattr_value(dev, "capacity");
                    std::string s = udev_device_get_sysattr_value(dev,"status");
                    std::string text = create_label_text(c,s);
                    mtx.lock();
                    batterylabel->rebuild()
                        ->text(text.c_str())
                        ->commence();
                    mtx.unlock();
                
                }
            
            }
        } 
        udev_enumerate_unref(enumerate);

        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
    }});
    th.detach();   
}

std::string hyprbar::Battery::create_label_text(std::string c, std::string s) {
    std::ostringstream txt;
    std::string status = "\uf240"; //full battery 
    int cap = std::atoi(c.c_str());
    if(cap < 75) {
        status = "\uf241";
    } else if(cap < 50) {
        status = "\uf242";
    } else if(cap < 25) {
        status = "\uf243";
    } else if(cap < 5) {
        status = "\uf244";
    }  
    if (s.compare("Charging") == 0) {
        status = "\uf5e7";
    } else if (s.compare("Not charging")==0) {
        status = "\uf1e6";
    }

    txt << c << '%' << status;
    return txt.str();
}

hyprbar::IModule* hyprbar::BatteryBuilder::create() {
    return new Battery();
}



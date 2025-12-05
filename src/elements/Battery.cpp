#include  "Battery.hpp"
#include <hyprtoolkit/element/Text.hpp>
#include <libudev.h>
#include <mutex>
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
                        std::ostringstream txt;
                        txt << udev_device_get_sysattr_value(dev, "capacity") << " % ["  << udev_device_get_sysattr_value(dev,"status") <<"]";
                        mtx.lock();
                        batterylabel->rebuild()->text(txt.str())->commence();
                        mtx.unlock();
                    }
            
                }
            } 
            udev_enumerate_unref(enumerate);

            std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        }
 
    });
    th.detach();   
}


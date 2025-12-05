#include  "Battery.hpp"
#include <hyprtoolkit/element/Text.hpp>
#include <libudev.h>
#include <sstream>
#include <thread>


hyprbar::Battery::~Battery() {
    /* Cleanup (unreachable in this infinite loop, but good practice for finite programs) */
    udev_monitor_unref(mon);
    udev_unref(udev);
}

hyprbar::Battery::Battery() {
    layout = CRowLayoutBuilder::begin()
        ->gap(2)
        ->commence();
    auto batterylabel = CTextBuilder::begin()
       ->text("NO BATTERY")
       ->commence(); 
    int fd;

    layout->addChild(batterylabel);

    /* Create the udev library context */
    udev = udev_new();
    if (!udev) {
        spdlog::error("Cannot create udev context.\n");
        exit(EXIT_FAILURE);
    }

    mon = udev_monitor_new_from_netlink(udev, "kernel");
    if (!mon) {
        spdlog::error("Cannot create udev monitor.\n");
        udev_unref(udev);
        exit(EXIT_FAILURE);
    }

    /* Filter the monitor to only receive events from the "power_supply" subsystem */
    if (udev_monitor_filter_add_match_subsystem_devtype(mon, "power_supply", NULL) < 0) {
        spdlog::error("Failed to add monitor filter.\n");
        udev_monitor_unref(mon);
        udev_unref(udev);
        exit(EXIT_FAILURE);
    }

    /* Enable the monitor */
    if (udev_monitor_enable_receiving(mon) < 0) {
        spdlog::error("Failed to enable receiving events.\n");
        udev_monitor_unref(mon);
        udev_unref(udev);
        exit(EXIT_FAILURE);
    }

    /* Get the file descriptor for the monitor */
    fd = udev_monitor_get_fd(mon);

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

                batterylabel->rebuild()->text(txt.str())->commence();
            }
            udev_device_unref(dev);
        }
    } 

    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    
    std::thread th([batterylabel, fd, this]() {
        
        struct pollfd fds[1];
        fds[0].fd = fd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;
        while(true) {
            
            int ret = poll(fds, 1, -1); /* Wait indefinitely for an event */

            if (ret > 0 && (fds[0].revents & POLLIN)) {
            
                spdlog::info("POLLIN");
                /* Receive the device event */
                struct udev_device *dev;
                dev = udev_monitor_receive_device(mon);
                if (dev) {
                    const char *action = udev_device_get_action(dev);
                    const char *devnode = udev_device_get_devnode(dev);
                    const char *status = udev_device_get_sysattr_value(dev, "status");
                    const char *capacity = udev_device_get_sysattr_value(dev, "capacity");
                    const char *devpath = udev_device_get_devpath(dev);

                    /* Only print events related to our specific battery, if possible */
                    /* The devpath can be checked to ensure it's BAT0 (or similar) */
                    if (strstr(devpath, "BAT0") != NULL) {
                        std::ostringstream txt;
                        txt << capacity << " % [" << status <<"]";

                        batterylabel->rebuild()->text(txt.str())->commence();
                    }

                    udev_device_unref(dev);
                } else {
                    fprintf(stderr, "No device received from udev monitor.\n");
                }
            }
        }
    });
    th.detach();   
}


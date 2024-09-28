#include <stdio.h>
#include <kcsdr.h>
#include <Windows.h>

int main() {
    printf("Hello World!\n");

    // Get a list of devices
    kcsdr_info_t* devices;
    int count = kcsdr_list_devices(&devices);
    if (count < 0) {
        fprintf(stderr, "Failed to list devices: %d\n", count);
        return -1;
    }
    else if (!count) {
        printf("No device found\n");
        return 0;
    }

    // Dump all available devices
    for (int i = 0; i < count; i++) {
        printf("- %s\n", devices[i].serial);
    }

    // Open the device
    kcsdr_t* dev;
    int err = kcsdr_open(&dev, devices[0].serial);
    if (err < 0) {
        fprintf(stderr, "Failed to open device: %d\n", err);
        return -1;
    }

    // Free the device list
    kcsdr_free_device_list(devices);
    

    Sleep(10000);

    // Close the device
    kcsdr_close(dev);

    return 0;
}
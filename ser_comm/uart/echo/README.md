# Device Tree Overlay + Kernel Module Demo

This project demonstrates how to:

- Compile a device tree overlay (`test_overlay.dts`)
- Load it using `dtoverlay`
- View it under `/proc/device-tree/`
- Test a kernel module that reads from the overlay

## Steps

1. Compile the device tree overlay:

dtc -@ -I dts -O dtb -o test_overlay.dtbo test_overlay.dts

2. Load the overlay:

sudo dtoverlay test_overlay.dtbo

3. View the loaded overlay in the device tree:

ls /proc/device-tree/my_device/

4. Read individual properties from the overlay:

sudo cat /proc/device-tree/my_device/label

5. Compile the kernel module:

make

6. Insert the kernel module:

sudo insmod dev_tree_test.ko

7. Check dmesg for output:

dmesg | tail


#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/of_device.h>
#include <linux/serdev.h>

static int ser_echo_probe(struct serdev_device *serdev);
static void ser_echo_remove(struct serdev_device *serdev);

static const struct of_device_id ser_echo_ids[] = {
    { .compatible = "decryptec,echo_dev" },
    {} // Sentinel
};
MODULE_DEVICE_TABLE(of, ser_echo_ids);

static int serdev_echo_recv(struct serdev_device *serdev, const unsigned char *buffer, size_t size)
{
    pr_info("echo - Received %ld bytes with \"%s\"\n", size, buffer);
    return serdev_device_write_buf(serdev, buffer, size);
}

static const struct serdev_device_ops ser_echo_ops = {
    .receive_buf = serdev_echo_recv,
};

static int ser_echo_probe(struct serdev_device *serdev)
{
    int status;

    pr_info("echo - Probe called\n");

    serdev_device_set_client_ops(serdev, &ser_echo_ops);

    status = serdev_device_open(serdev);
    if (status) {
        pr_err("echo - Error opening serial port\n");
        return -status;
    }

    pr_info("echo - Configuring UART settings\n");

    serdev_device_set_baudrate(serdev, 115200);
    serdev_device_set_flow_control(serdev, false);
    serdev_device_set_parity(serdev, SERDEV_PARITY_NONE);

    return 0;
}

static void ser_echo_remove(struct serdev_device *serdev)
{
    pr_info("echo - Remove called\n");
    serdev_device_close(serdev);
}

static struct serdev_device_driver serdev_device_driver = {
    .probe = ser_echo_probe,
    .remove = ser_echo_remove,
    .driver = {
        .name = "uart-echo",
        .of_match_table = ser_echo_ids,
    },
};

static int __init my_init(void)
{
    pr_info("echo - Loading driver\n");

    if (serdev_device_driver_register(&serdev_device_driver)) {
        pr_err("echo - Could not load driver\n");
        return -1;
    }

    pr_info("echo - Driver loaded successfully\n");
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("echo - Unloading driver\n");
    serdev_device_driver_unregister(&serdev_device_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Repeat back driver of UART port");


#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>

#define DRIVER_NAME "ads1115"
#define DRIVER_CLASS "ads1115_class"

#define I2C_BUS_AVAILABLE 1
#define ADS1115_ADDR 0x48
#define ADS1115_NAME "ADS1115"

static struct i2c_adapter *ads_i2c_adapter = NULL;
static struct i2c_client *ads_i2c_client = NULL;

static dev_t my_dev_nr;
static struct class *my_class;
static struct cdev my_device;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Don Modified");
MODULE_DESCRIPTION("Char driver for ADS1115 16-bit ADC");
MODULE_SUPPORTED_DEVICE("none");

static struct i2c_board_info ads_i2c_board_info = {
    I2C_BOARD_INFO(ADS1115_NAME, ADS1115_ADDR)
};

static const struct i2c_device_id ads_id[] = {
    { ADS1115_NAME, 0 },
    {}
};

static struct i2c_driver ads_driver = {
    .driver = {
        .name = ADS1115_NAME,
        .owner = THIS_MODULE,
    },
    .id_table = ads_id,
};

static int ads1115_read_channel(int channel) {
    u16 config;
    u16 raw;
    int ret;

    // Base config: single-shot, 128SPS, single-ended, FSR ±6.144V
    config = 0x8000 | (0x4000 + (channel << 12)) | 0x0083;

    // Write config to start conversion
    i2c_smbus_write_word_data(ads_i2c_client, 0x01, swab16(config));
    msleep(10); // Wait for conversion

    ret = i2c_smbus_read_word_data(ads_i2c_client, 0x00);
    if (ret < 0) return ret;

    raw = swab16(ret); // ADS1115 sends MSB first

    return (s16)raw; // Convert to signed
}

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
    int to_copy, not_copied, delta;
    char out_string[20];
    int adc_val;

    to_copy = min(count, sizeof(out_string));

    adc_val = ads1115_read_channel(0);  // Channel AIN0
    snprintf(out_string, sizeof(out_string), "%d\n", adc_val);

    not_copied = copy_to_user(user_buffer, out_string, to_copy);
    delta = to_copy - not_copied;

    return delta;
}

static int driver_open(struct inode *deviceFile, struct file *instance) {
    printk("ADS1115 - Open\n");
    return 0;
}

static int driver_close(struct inode *deviceFile, struct file *instance) {
    printk("ADS1115 - Close\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
};

static int __init ads1115_init(void) {
    int ret;

    if (alloc_chrdev_region(&my_dev_nr, 0, 1, DRIVER_NAME) < 0)
        return -1;

    if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
        goto ClassError;

    if (device_create(my_class, NULL, my_dev_nr, NULL, DRIVER_NAME) == NULL)
        goto FileError;

    cdev_init(&my_device, &fops);

    if (cdev_add(&my_device, my_dev_nr, 1) == -1)
        goto AddError;

    ads_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (!ads_i2c_adapter)
        goto AdapterError;

    ads_i2c_client = i2c_new_client_device(ads_i2c_adapter, &ads_i2c_board_info);
    i2c_put_adapter(ads_i2c_adapter);

    if (!ads_i2c_client)
        goto AdapterError;

    if (i2c_add_driver(&ads_driver) != 0)
        goto DriverError;

    printk("ADS1115 driver loaded.\n");
    return 0;

DriverError:
    i2c_unregister_device(ads_i2c_client);
AdapterError:
    cdev_del(&my_device);
AddError:
    device_destroy(my_class, my_dev_nr);
FileError:
    class_destroy(my_class);
ClassError:
    unregister_chrdev_region(my_dev_nr, 1);
    return -1;
}

static void __exit ads1115_exit(void) {
    i2c_unregister_device(ads_i2c_client);
    i2c_del_driver(&ads_driver);
    cdev_del(&my_device);
    device_destroy(my_class, my_dev_nr);
    class_destroy(my_class);
    unregister_chrdev_region(my_dev_nr, 1);
    printk("ADS1115 driver unloaded.\n");
}

module_init(ads1115_init);
module_exit(ads1115_exit);

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/string.h>

// Meta
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Simple module using sysfs class interface");

static dev_t dev_nr;
static struct class *my_class;
static struct device *my_dev;

static char text[64] = "Hello World!";
static int answer = 42;

ssize_t text_store(struct device *d, struct device_attribute *a, const char *buf, size_t len)
{
    size_t to_copy = len < sizeof(text) ? len : sizeof(text) - 1;

    strncpy(text, buf, to_copy);
    text[to_copy] = '\0'; // Ensure null termination
    return to_copy;
}

ssize_t text_show(struct device *d, struct device_attribute *a, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%s", text);
}

ssize_t answer_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &answer);
    return count;
}

ssize_t answer_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%d\n", answer);
}

DEVICE_ATTR(text, 0660, text_show, text_store);
DEVICE_ATTR(answer, 0660, answer_show, answer_store);

static int __init my_init(void)
{
    pr_info("sysfs - Init\n");

    int status = alloc_chrdev_region(&dev_nr, 0, MINORMASK + 1, "my-dev");
    if (status != 0) {
        pr_err("sysfs - Error allocating device number\n");
        return status;
    }

    my_class = class_create(THIS_MODULE, "my_class");
    if (IS_ERR(my_class)) {
        pr_err("sysfs - Error creating class\n");
        status = PTR_ERR(my_class);
        goto free_dev_nr;
    }

    my_dev = device_create(my_class, NULL, dev_nr, NULL, "my_dev%d", MINOR(dev_nr));
    if (IS_ERR(my_dev)) {
        pr_err("sysfs - Error creating device\n");
        status = PTR_ERR(my_dev);
        goto free_class;
    }

    status = device_create_file(my_dev, &dev_attr_text);
    if (status != 0) {
        pr_err("sysfs - Error creating text attribute\n");
        goto free_dev;
    }

    status = device_create_file(my_dev, &dev_attr_answer);
    if (status != 0) {
        pr_err("sysfs - Error creating answer attribute\n");
        goto free_text;
    }

    return 0;

free_text:
    device_remove_file(my_dev, &dev_attr_text);
free_dev:
    device_destroy(my_class, dev_nr);
free_class:
    class_destroy(my_class);
free_dev_nr:
    unregister_chrdev_region(dev_nr, MINORMASK + 1);
    return status;
}

static void __exit my_exit(void)
{
    pr_info("sysfs - Exit\n");

    device_remove_file(my_dev, &dev_attr_answer);
    device_remove_file(my_dev, &dev_attr_text);
    device_destroy(my_class, dev_nr);
    class_destroy(my_class);
    unregister_chrdev_region(dev_nr, MINORMASK + 1);
}

module_init(my_init);
module_exit(my_exit);

#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

static int dev_tree_test(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);

static const struct of_device_id my_driver_ids[] = {
    {
        .compatible = "decryptec,mydev",  // ❗ Remove space after comma
    },
    {} // sentinel
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct platform_driver my_driver = {
    .probe = dev_tree_test,
    .remove = dt_remove,
    .driver = {
        .name = "my_device_driver",
        .of_match_table = my_driver_ids
    },
};

static int dev_tree_test(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    const char *label;
    u32 my_value; 
    int ret;

    pr_info("dev_tree_test - Probe called\n");

    // Check device properties
    if (!device_property_present(dev, "label")) {
        pr_err("dev_tree_test - Property 'label' not found\n");
    }
    if (!device_property_present(dev, "my_value")) { 
        pr_err("dev_tree_test - Property 'my_value' not found\n");
    }

    ret = device_property_read_string(dev, "label", &label);
    if (ret) {
        pr_info("dev_tree_test - Unable to read 'label'\n");
        return -EINVAL;
    }
    pr_info("dev_tree_test - label: %s\n", label);

    ret = device_property_read_u32(dev, "my_value", &my_value);
    if (ret) {
        pr_info("dev_tree_test - Unable to read 'my_value'\n");
        return -EINVAL;
    }
    pr_info("dev_tree_test - my_value: %u\n", my_value); 

    return 0;
}

static int dt_remove(struct platform_device *pdev) 
{
    pr_info("dev_tree_test - Remove called\n");
    return 0;
}

static int __init my_init(void)
{
    pr_info("dev_tree_test - Load driver\n");
    if (platform_driver_register(&my_driver)) {  
        pr_err("dev_tree_test - Could not load driver\n");
        return -1;
    }
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("dev_tree_test - Unload driver\n");
    platform_driver_unregister(&my_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Simple device tree test");

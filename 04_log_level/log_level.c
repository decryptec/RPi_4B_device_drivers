#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
    printk(KERN_INFO "log_level - hello world\n");
    printk(KERN_WARNING "log_level - Warning msg\n");
    printk(KERN_ALERT "log_level - Alert msg\n");
    printk(KERN_DEBUG "log_level - Debug msg: %d\n", 42);
    return 0;
}

static void __exit my_exit(void)
{
	pr_emerg("log_level - Module unloaded\n");
	pr_err("log_level - Error\n");
	pr_info("log_level - last info\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Hello world Linux Kernal Module");

#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
    printk("hello world\n");
    return 0;
}

static void __exit my_exit(void)
{
    printk("Bye world\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Hello world Linux Kernal Module");
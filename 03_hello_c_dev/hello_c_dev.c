#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major = 0;

static ssize_t my_read (struct file *f, char __user *u, size_t l, loff_t *o){
	printk("hello_c_dev - Read is called\n");
	return 0;
}

static struct file_operations fops = {
	.read = my_read
};

static int __init my_init(void)
{
    major = register_chrdev(0, "hello_c_dev", &fops); 
    if (major < 0) {
        printk("hello_c_dev - error registering chrdev\n");
        return major;
    }
    printk("hello_c_dev - Major Device Number: %d\n", major);
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev(major, "hello_cdev");
    printk("hello_c_dev - Exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Hello world driver via character device");

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major;

static int my_open(struct inode *inode, struct file *filp)
{
	pr_info("open_release_cdev - Major: %d, Minor %d\n", imajor(inode), iminor(inode));

	pr_info("open_release_cdev - filp->f_pos: %lld\n", filp->f_pos);
	pr_info("open_release_cdev - filp->f_mode: 0x%x\n", filp->f_mode);
	pr_info("open_release_cdev - filp->f_flags: 0x%x\n", filp->f_flags);

	return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
	pr_info("open_release_cdev - File is closed");
	return 0;
}

static struct file_operations fops = {
	.open = my_open,
	.release = my_release,
};

static int __init my_init(void)
{
    major = register_chrdev(0, "open_release_cdev", &fops); 
    if (major < 0) {
        pr_info("open_release_cdev - error registering chrdev\n");
        return major;
    }
    pr_info("open_release_cdev - Major Device Number: %d\n", major);
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev(major, "open_release_cdev");
    pr_info("open_release_cdev - Exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Driver with open and release fops via character device");

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>

#include "ioctl_test.h"

static int major;

static int my_open(struct inode *inode, struct file *filp)
{
	pr_info("ioctl - Major: %d, Minor %d\n", imajor(inode), iminor(inode));

	pr_info("ioctl - filp->f_pos: %lld\n", filp->f_pos);
	pr_info("ioctl - filp->f_mode: 0x%x\n", filp->f_mode);
	pr_info("ioctl - filp->f_flags: 0x%x\n", filp->f_flags);

	return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
	pr_info("ioctl - File is closed");
	return 0;
}

/* Global Variable for reading and writing */
int32_t answer = 42;

static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg){
	struct mystruct test;
	switch(cmd){
		case WR_VALUE:
			if(copy_from_user(&answer, (int32_t *) arg, sizeof(answer))) 
				printk("ioctl_example - Error copying data from user!\n");
			else
				printk("ioctl_example - Update the answer to %d\n", answer);
			break;
		case RD_VALUE:
			if(copy_to_user((int32_t *) arg, &answer, sizeof(answer))) 
				printk("ioctl_example - Error copying data to user!\n");
			else
				printk("ioctl_example - The answer was copied!\n");
			break;
		case GREETER:
			if(copy_from_user(&test, (struct mystruct *) arg, sizeof(test))) 
				printk("ioctl_example - Error copying data from user!\n");
			else
				printk("ioctl_example - %d greets to %s\n", test.repeat, test.name);
			break;
	}
	return 0;
}

static struct file_operations fops = {
	.open = my_open,
	.release = my_release,
	.unlocked_ioctl = my_ioctl,
};

static int __init my_init(void)
{
	major = register_chrdev(0, "ioctl", &fops); 
	if (major < 0) {
		pr_info("ioctl - error registering chrdev\n");
		return major;
	}
	pr_info("ioctl - Major Device Number: %d\n", major);
	return 0;
}

static void __exit my_exit(void)
{
	unregister_chrdev(major, "ioctl_cdev");
	pr_info("ioctl - Exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Driver with open and release fops via character device");

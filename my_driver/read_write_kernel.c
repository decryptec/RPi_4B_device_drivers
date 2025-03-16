#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

static struct proc_dir_entry *custom_proc_node;

static ssize_t my_read(struct file* file_ptr, 
		char *usr_buffer, 
		size_t count,
		loff_t* offset){
	char msg[] = "Ack\n";
	size_t len = strlen(msg);
	int result;

	result = copy_to_user(usr_buffer, msg, len);

	*offset += len;

	if (*offset > len)
	{
		return 0;
	}

	printk(KERN_INFO "my_read\n");
	return len;
}

struct proc_ops driver_proc_ops = {
	.proc_read=my_read
};

static int __init my_init(void)
{
    printk(KERN_INFO "my_driver_init - Entry\n");

    custom_proc_node = proc_create("my_driver",
		    0,
		    NULL,
		    &driver_proc_ops);

    printk(KERN_INFO "my_driver_init - Exit\n");

    return 0;
}

static void __exit my_exit(void)
{
    printk(KERN_INFO "my_driver_exit - Entry\n");

    proc_remove(custom_proc_node);

    printk(KERN_INFO "my_driver_exit - Exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("Basic read loadable kernel module");

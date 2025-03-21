#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major = 0;
static char buffer[64];

static ssize_t my_read (struct file *filp, char __user *user_buf, size_t len, loff_t *offset){
	int not_copied, delta, to_copy = (len + *offset) < sizeof(buffer) ? len : (sizeof(buffer) - *offset);

	pr_info("hello_cdev - Read is called. Reading %ld bytes, but only copy %d bytes. Offset: %lld\n", len, to_copy, *offset);

	if(*offset >=sizeof(buffer))
		return 0;
	not_copied = copy_to_user(user_buf, &buffer[*offset], to_copy);
	delta = to_copy - not_copied;
	if(not_copied)
		pr_warn("hello_cdev - Could only copy %d bytes\n", delta);
	*offset += delta;
	return delta;
}



static ssize_t my_write (struct file *filp, const char __user *user_buf, size_t len, loff_t *offset){
	int not_copied, delta, to_copy = (len+ *offset) < sizeof(buffer) ? len : (sizeof(buffer) - *offset);

	pr_info("hello_cdev - Write is called, read %ld bytes, but only copy %d bytes. Offset: %lld\n", len, to_copy, *offset);
	if(*offset >=sizeof(buffer))
		return 0;
	not_copied = copy_from_user(&buffer[*offset], user_buf, to_copy);
	delta = to_copy - not_copied;
	if(not_copied)
		pr_warn("hello_cdev - Could only copy %d, bytes\n", delta);
	*offset +=delta;
	return delta;
}
static struct file_operations fops = {
	.read = my_read,
	.write = my_write
};

static int __init my_init(void)
{
	major = register_chrdev(0, "hello_c_dev", &fops); 
	if (major < 0) {
		pr_info("hello_c_dev - error registering chrdev\n");
		return major;
	}
	pr_info("hello_c_dev - Major Device Number: %d\n", major);
	return 0;
}

static void __exit my_exit(void)
{
	unregister_chrdev(major, "hello_cdev");
	pr_info("hello_c_dev - Exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Decryptec");
MODULE_DESCRIPTION("A read and write hello world via char device");

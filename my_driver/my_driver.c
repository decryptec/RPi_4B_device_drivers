#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

#define MAX_BUFFER_SIZE 1024

static struct proc_dir_entry *custom_proc_node;

static char proc_buffer[MAX_BUFFER_SIZE];
static size_t buffer_size = 0;

static ssize_t my_read(struct file *file_ptr,
                       char __user *usr_buffer,
                       size_t count,
                       loff_t *offset) {
    size_t len = buffer_size; // Use the size of the data in proc_buffer
    int result;

    // Check if offset exceeds the length of the buffer (EOF condition)
    if (*offset >= len) {
        return 0; // Return 0 to indicate EOF
    }

    // Adjust `len` if the user requests fewer bytes than available
    if (count < len - *offset) {
        len = count;
    } else {
        len = len - *offset;
    }

    // Copy data from the kernel buffer to the user buffer
    result = copy_to_user(usr_buffer, proc_buffer + *offset, len);
    if (result != 0) {
        printk(KERN_ERR "Failed to copy data to user space.\n");
        return -EFAULT; // Error code for failure
    }

    *offset += len; // Update the file offset

    printk(KERN_INFO "my_read: Sent %zu bytes: %s\n", len, proc_buffer);
    return len; // Return the number of bytes read
}

static ssize_t my_write(struct file *file_ptr, const char __user *usr_buffer, size_t count, loff_t *offset) {
    // Ensure we don't overflow the buffer
    if (count > MAX_BUFFER_SIZE) {
        printk(KERN_WARNING "Input exceeds buffer size. Data will be truncated.\n");
        count = MAX_BUFFER_SIZE; // Truncate to fit buffer
    }

    // Copy data from user space to kernel space
    if (copy_from_user(proc_buffer, usr_buffer, count) != 0) {
        printk(KERN_ERR "Failed to copy data from user space.\n");
        return -EFAULT; // Error code for failed copy
    }

    buffer_size = count; // Update the buffer size
    proc_buffer[buffer_size] = '\0'; // Null-terminate the string

    printk(KERN_INFO "my_write: Stored %zu bytes: %s\n", buffer_size, proc_buffer);
    return count; // Return the number of bytes written
}

struct proc_ops driver_proc_ops = {
	.proc_read=my_read,
	.proc_write=my_write
};

static int __init my_init(void)
{
    printk(KERN_INFO "my_driver_init - Entry\n");

    custom_proc_node = proc_create("my_driver",
		    0666,
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

#include "misc_device.h"

#include <linux/miscdevice.h>

static struct file_operations device_fops = {
	.owner = THIS_MODULE,
};

static struct miscdevice misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "keyboard_counter",
	.fops = &device_fops,
	.mode = 0666,
};

int read_device_register(ssize_t (*read_function)(struct file *, char *, size_t,
						  loff_t *))
{
	int ret;
	device_fops.read = read_function;

	ret = misc_register(&misc_device);

	if (ret) {
		printk(KERN_ERR
		       "=== keyboard counter logger: unable to register misc device\n");
	}

	return ret;
}

void read_device_deregister(void)
{
	misc_deregister(&misc_device);
}

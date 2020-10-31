#ifndef __MISC_DEVICE_H
#define __MISC_DEVICE_H

#include <linux/fs.h>

int read_device_register(ssize_t (*read_function)(struct file *, char *, size_t,
						  loff_t *));
void read_device_deregister(void);

#endif // __MISC_DEVICE_H

#include "counter.h"
#include "irq_module.h"
#include "irq.h"
#include "job.h"
#include "misc_device.h"

#include <asm/uaccess.h>

#include <linux/spinlock.h>

#define BUFFER_SIZE 100
#define WINDOW 60

static spinlock_t lock;
static char result_buffer[BUFFER_SIZE]; // protected by lock;
static atomic_t counter;

static ssize_t logger_read(struct file *file, char *buffer, size_t count,
			   loff_t *ppos)
{
	size_t len;
	char buf[BUFFER_SIZE];

	spin_lock(&lock);
	memcpy(buf, result_buffer, BUFFER_SIZE);
	spin_unlock(&lock);

	len = strlen(buf);

	if (count < len) {
		return -EINVAL;
	}

	if (*ppos != 0) {
		return 0;
	}

	if (copy_to_user(buffer, buf, len)) {
		return -EINVAL;
	}

	*ppos = len;
	return len;
}

static void update_result_buffer(void)
{
	int r = window_counter_agg(&counter);

	spin_lock(&lock);
	memset(result_buffer, 0, BUFFER_SIZE);
	sprintf(result_buffer, "%d", r);
	spin_unlock(&lock);
}

static void update_counter(void)
{
	window_counter_inc(&counter);
}

static int __init irq_module_init(void)
{
	spin_lock_init(&lock);
	window_counter_init(&counter);
	result_buffer[0] = '0';
	result_buffer[1] = '\0';

	if (keyboard_interrupt_init(update_counter)) {
		return -EINVAL;
	}

	start_periodic_job(update_result_buffer, WINDOW);

	if (read_device_register(logger_read)) {
		keyboard_interrupt_destroy();
		stop_periodic_job();
		return -EINVAL;
	}

	return 0;
}

static void __exit irq_module_exit(void)
{
	read_device_deregister();
	keyboard_interrupt_destroy();
	stop_periodic_job();
}

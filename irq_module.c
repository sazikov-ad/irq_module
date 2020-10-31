#include "counter.h"
#include "irq_module.h"
#include "irq.h"
#include "job.h"
#include "misc_device.h"

#include <asm/uaccess.h>

#define BUFFER_SIZE 100
#define WINDOW 60

static atomic_t result_buffer;
static atomic_t counter;

static ssize_t logger_read(struct file *file, char *buffer, size_t count,
			   loff_t *ppos)
{
	size_t len;
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);

	sprintf(buf, "%d", atomic_read(&result_buffer));

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
	atomic_set(&result_buffer, window_counter_agg(&counter));
}

static void update_counter(void)
{
	window_counter_inc(&counter);
}

static int __init irq_module_init(void)
{
	window_counter_init(&counter);
	atomic_set(&result_buffer, 0);

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

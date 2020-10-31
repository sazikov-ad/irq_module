#include "job.h"

#include <linux/atomic.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

static struct workqueue_struct *queue;

static void (*task)(void);
static unsigned long backoff;

static atomic_t should_stop;

static void workqueue_function(struct work_struct *work);

static void schedule_iteration(void)
{
	struct delayed_work *work = (struct delayed_work *)kmalloc(
		sizeof(struct delayed_work), GFP_KERNEL);
	INIT_DELAYED_WORK(work, workqueue_function);
	queue_delayed_work(queue, work, backoff * HZ);
}

static void workqueue_function(struct work_struct *work)
{
	if (atomic_read(&should_stop) == 0) {
		task();
		schedule_iteration();
	}

	kfree((struct delayed_work *)work);
}

void start_periodic_job(void (*job)(void), unsigned long period)
{
	queue = create_workqueue("periodic action queue");
	atomic_set(&should_stop, 0);

	task = job;
	backoff = period;

	schedule_iteration();
}

void stop_periodic_job(void)
{
	atomic_set(&should_stop, 1);
	flush_workqueue(queue);
	destroy_workqueue(queue);
}

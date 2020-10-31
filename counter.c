#include "counter.h"

#include <linux/atomic.h>

void window_counter_init(atomic_t *counter)
{
	atomic_set(counter, 0);
}

void window_counter_inc(atomic_t *counter)
{
	atomic_inc(counter);
}

int window_counter_agg(atomic_t *counter)
{
	return atomic_xchg(counter, 0);
}

#ifndef __COUNTER_H
#define __COUNTER_H

#include <linux/types.h>

void window_counter_init(atomic_t *counter);

void window_counter_inc(atomic_t *counter);
int window_counter_agg(atomic_t *counter);

#endif // __COUNTER_H

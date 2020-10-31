#include "irq.h"

#include <linux/interrupt.h>

static int irq = 1, dev_id;

static void (*irq_action)(void);

static irqreturn_t keybaord_interrupt(int irq, void *device_id)
{
	irq_action();
	return IRQ_NONE;
}

int keyboard_interrupt_init(void (*action)(void))
{
	irq_action = action;
	return request_irq(irq, keybaord_interrupt, IRQF_SHARED,
			   "keyboard interrupt counter", &dev_id);
}

void keyboard_interrupt_destroy(void)
{
	synchronize_irq(irq);
	free_irq(irq, &dev_id);
}

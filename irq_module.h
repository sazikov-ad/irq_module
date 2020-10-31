#ifndef __IRQ_MODULE_H
#define __IRQ_MODULE_H

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrey Sazikov <sazikov.ad@phystech.edu>");
MODULE_VERSION("1.0");

static int __init irq_module_init(void);
static void __exit irq_module_exit(void);

module_init(irq_module_init);
module_exit(irq_module_exit);

#endif // __IRQ_MODULE_H

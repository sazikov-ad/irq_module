#ifndef __IRQ_H
#define __IRQ_H

int keyboard_interrupt_init(void (*on_interrupt)(void));
void keyboard_interrupt_destroy(void);

#endif // __IRQ_H

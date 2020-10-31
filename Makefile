obj-m += keyboard_counter.o

keyboard_counter-objs := counter.o job.o irq.o irq_module.o misc_device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

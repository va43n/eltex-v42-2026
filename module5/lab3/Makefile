obj-m += my_kbleds.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

format:
	clang-format -i my_kbleds.c
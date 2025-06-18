# Name of our kernel object
obj-m += http_logger.o

# The command to build the module
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# The command to clean up compiled files
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

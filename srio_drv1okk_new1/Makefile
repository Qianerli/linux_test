#cpu架构
ARCH = sparc
#内核源代码路径
KERNEL_DIR=/opt/linuxbuild-2.0.0/linux/build-linux
#交叉编译器的路径(前缀)
CROSS_PATH=/opt/sparc-gaisler-linux4.9/bin/sparc-gaisler-linux4.9-

#编译成模块
obj-m		:=		srio_drv.o

default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_PATH) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod* *.ord* *.sy* .*cmd .tmp*


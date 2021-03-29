#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/dma-mapping.h>





#define COPY_NO_DMA    0
#define COPY_BY_DMA    1

#define COPY_BUFF_SIZE (1024*1)

static int major = 0;

static char *src_addr;              //源物理地址
static dma_addr_t src_phy_addr;    //源虚拟地址

static char *dst_addr;              //目标物理地址
static dma_addr_t dst_phy_addr;    //目标虚拟地址

static struct class *cls;
static struct class_device    *cls_dev;



static unsigned long dma_mem_alloc(int size)
{
	int order = get_order(size);
	return __get_dma_pages(GFP_KERNEL, order);
}


static int dma_drv_open(struct inode *inode, struct file *file)
{

/*
    printk("-1-dma_drv_open\n");
    src_addr = (char *)dma_mem_alloc(COPY_BUFF_SIZE);
    if(src_addr == NULL)
    {
        printk("can't alloc buffer for src\n");
        return -ENOMEM;
    }
    printk("src_addr = %x\n",src_addr);

    dst_addr = (char *)dma_mem_alloc(COPY_BUFF_SIZE);
    if(dst_addr == NULL)
    {
        printk("can't alloc buffer for src\n");
        return -ENOMEM;
    }
    printk("dst_addr = %x\n",dst_addr);
*/



    src_addr = (char *)dma_alloc_coherent(NULL, COPY_BUFF_SIZE, &src_phy_addr, GFP_KERNEL|GFP_DMA);
    if(src_addr == NULL)
    {
        printk("can't alloc buffer for src\n");
        return -ENOMEM;
    }
    printk("src_addr = %x\n",src_addr);
    printk("src_phy_addr = %x\n",src_phy_addr);
    
    dst_addr = (char *)dma_alloc_coherent(NULL, COPY_BUFF_SIZE, &dst_phy_addr, GFP_KERNEL|GFP_DMA);
    if(dst_addr == NULL)
    {
        printk("can't alloc buffer for dst\n");
        return -ENOMEM;
    }
    printk("dst_addr = %x\n",dst_addr);
    printk("dst_phy_addr = %x\n",dst_phy_addr);


    printk("-2-dma_drv_open\n");
    return 0;
}

static long dma_drv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int n;

    memset(src_addr, 0x55, COPY_BUFF_SIZE);
    memset(dst_addr, 0xAA, COPY_BUFF_SIZE);
    
    switch(cmd)
    {
        case COPY_NO_DMA:
            for(n = 0; n < COPY_BUFF_SIZE; n++)
            {
                //printk("before: dst_addr[%d] = %x,src_addr[%d] = %x\n",n,dst_addr[n],n,src_addr[n]);

                dst_addr[n] = src_addr[n];

                //printk("later: dst_addr[%d] = %x,src_addr[%d] = %x\n",n,dst_addr[n],n,src_addr[n]);

            }
                

            if(memcmp(src_addr, dst_addr, COPY_BUFF_SIZE) == 0)
            {
                printk("MEM_CPY_NO_DMA OK\n");
            }
            else
            {
                printk("MEM_CPY_NO_DMA ERROR\n");
            }




            break;

        default:
            break;
    }

    return 0;
}

static int dma_drv_close(struct inode *inode, struct file *file)
{
    printk("dma_drv_close\n");
    //dma_free_coherent(NULL, COPY_BUFF_SIZE, src_addr, &src_phy_addr);
    //dma_free_coherent(NULL, COPY_BUFF_SIZE, dst_addr, &dst_phy_addr);

    return 0;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open  = dma_drv_open,
    .unlocked_ioctl = dma_drv_ioctl,
    .release = dma_drv_close,
};

static int __init dma_drv_init(void)
{
    major = register_chrdev(0, "dma_dev", &fops);
    
    cls = class_create(THIS_MODULE, "dma_cls");
    cls_dev = device_create(cls, NULL, MKDEV(major, 0), NULL, "dma_drv");

    return 0;
}

static void __exit dma_drv_exit(void)
{
    unregister_chrdev(major, "dma_dev");

    device_unregister(cls_dev);
    class_destroy(cls);
}

module_init(dma_drv_init);
module_exit(dma_drv_exit);

MODULE_LICENSE("GPL");
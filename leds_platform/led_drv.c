#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/leds.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/ioctl.h>

#include <linux/miscdevice.h>
#include <linux/io.h>

//ioctl命令
#define LED_PLATFORM _IO('a',0)

//资源地址
struct resource *led_res = NULL;
//IO映射
void __iomem *led_base = NULL;


//led设备的ioctl函数实现
static long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	int ret;
	unsigned int val_led = 0;


	//在内核中还需要将,printk的打印放在变量定义之后
	//printk("enter led_ioctl!\n");	
		
	switch(cmd){
		//1.控制led的点亮
		case LED_PLATFORM:
			//printk("LED_OFF!\n");

			ret = copy_from_user(&val_led, (const char*)arg, sizeof(val_led)); //copy_to_user();
			//printk("led_ioctl-ret = -%d-\n",ret);			
			//应用层传过来的数据
			//printk("kennel-val_led = %x\n",val_led);		
				
			iowrite32(val_led,led_base+0x4);	
			//printk(">===========ioctl_off_end!=============<\n");			
			break;

		default:
			return -EINVAL;
	}	
	
	
	return 0;
}

static int led_open(struct inode *inode, struct file *filp)
{
	printk("led_open\n");
    return 0;
}

static int led_release(struct inode *inode, struct file *filp)
{
	printk("led_release\n");
    return 0;
}

//LED设备的write函数实现
static ssize_t led_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	printk("led_write\n");   
    return 0;
}

static ssize_t led_read(struct file *file,char __user *buf, size_t count, loff_t * ppos)
{
	printk("led_read\n");   
    return 0;
}

//定义并初始化LED设备的操作集
static const struct file_operations led_ops = 
{
    .owner      = THIS_MODULE,
    .open       = led_open,
    .write      = led_write,
	.read  		= led_read, 
    .unlocked_ioctl = led_ioctl,//ioctl接口
    .release    = led_release,
};


//分配初始化miscdevice
struct miscdevice led_dev = {
	.minor = MISC_DYNAMIC_MINOR,//系统分配次设备号
	.name = "led",//设备文件名
	.fops = &led_ops,//操作函数集合
};


//led设备的probe函数实现
static int led_probe(struct platform_device *pdev)
{
	int ret;
	
	//注册miscdevice
	ret = misc_register(&led_dev);
	if(ret<0){
		printk("misc_register failed!\n");
		goto failure_misc_register;
	}

	//获取pwm和GPIO资源
	led_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(!led_res){
		printk("platform_get_resource failed!\n");
		goto failure_get_resource;
	}

	//映射IO内存
	led_base = ioremap(led_res->start,led_res->end-led_res->end);
	if(IS_ERR(led_base)){
		printk("ioremap failed!\n");
		ret = -ENOMEM;
		goto failure_pwm_ioremap;
	}

	printk("led probe!\n");

	return 0;

failure_pwm_ioremap:
failure_get_resource:
	misc_deregister(&led_dev);
failure_misc_register:
	return ret;     
}
    
int led_remove(struct platform_device *pdev)
{
	//解除IO映射
	iounmap(led_base);

	//注销miscdevice	
	misc_deregister(&led_dev);

	return 0;
}

//名字数组
struct platform_device_id led_table[] = {
	{"led"},
	{"led1"},
	{"led2"},
	{""}//结束成员
};

//分配初始化platform_driver
static struct platform_driver led_drv = 
{
	.id_table = led_table,
    .probe = led_probe,
    .remove = led_remove,
    .driver = 
    {
        .name  = "led",//用于匹配name
        .owner = THIS_MODULE,
    },
};

//注册LED驱动
static int __init led_driver_init(void)
{
    return platform_driver_register(&led_drv);
}

//注销LED驱动
static void __exit led_driver_exit(void)
{
    platform_driver_unregister(&led_drv);
}

module_init(led_driver_init);
module_exit(led_driver_exit);

MODULE_AUTHOR("Lin");
MODULE_DESCRIPTION("led driver for x210");
MODULE_LICENSE("GPL");
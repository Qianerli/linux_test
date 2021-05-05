#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>


#define GPIO_ADDR   0x80000a00 		//GPIO寄存器地址
#define PORT_DATA  	0x00000000		//输出寄存器偏移量
#define PORT_OUTPUT 0x00000004		//数据寄存器偏移量
#define PORT_DIRM  	0x00000008  	//方向寄存器偏移量



//定义并初始化LED设备的相关资源
static struct resource led_res[] = 
{
	[0] = {
			.start 	= GPIO_ADDR+PORT_DATA,
			.end 	= GPIO_ADDR+PORT_DIRM-1,
			.flags 	= IORESOURCE_MEM,
	},
};


void led_release(struct device *dev)
{
	printk("enter led_release!\n");
}

/*
static struct led_plat_date led_platdate = 
{
	.flags	= 	NULL,
}
*/

//定义并初始化LED设备信息
static struct platform_device led_dev = 
{
    .name = "led",             //设备名称
    .id = -1,                  //设备数量，-1表示只有一个设备
    .resource = led_res, //资源指针
    .num_resources = ARRAY_SIZE(led_res), //计算设备资源数量
    .dev = 
    {
        .release = led_release,	//删除节点,自动调用,必须实现
		//.platform_date = led_platdate,	//传递额外数据信息
    },
};

//注册LED设备
static int __init led_device_init(void)
{
    return platform_device_register(&led_dev);
}

//注销LED设备
static void __exit led_device_exit(void)
{
    platform_device_unregister(&led_dev);
}

module_init(led_device_init);
module_exit(led_device_exit);

MODULE_AUTHOR("Lin");
MODULE_DESCRIPTION("led device for x210");
MODULE_LICENSE("GPL");
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>


#define MINOR_1553B 0


/*---------------------配置1553b寄存器--------------------*/

#define REG_DDR   	0x60000000 					//1553B寄存器首地址
#define RAM_DDR   	0x60020000 					//1553B_RAM寄存器首地址

#define CONTROL_REG_DDR  		0x00000000		//控制寄存器
#define STATUS_REG_DDR 			0x00000002		//操作和状态寄存器
#define COMMAND_REG_DDR			0x00000004		//当前命令寄存器
#define SHORTMASK_REG_DDR  		0x00000006		//中断屏蔽寄存器
#define SHORTOPRATE_REG_DDR 	0x00000008		//待处理中断寄存器
#define SHORTPNT_REG_DDR 		0x0000000a		//中断指针寄存器
#define TEST_REG_DDR			0x0000000c		//内置测试寄存器

#define BC_FM_RT_TT_DDR 		0x0000000e		//小帧计时器寄存器+时间标签寄存器
#define BC_CP_RT_DP_DDR 		0x00000010		//命令块指针寄存器+描述符指针寄存器

#define RT_STATUS_DDR			0x00000012		//1553A/B状态字寄存器
#define RT_LEG_DDR				0x00000014		//RT合法寄存器

#define MONITOR_CMDPNT_DDR  	0x00000016		//监控命令指针寄存器
#define MONITOR_BLK_DDR 		0x00000018		//监控模块寄存器
#define MONITOR_CNT_DDR			0x0000001a		//监控计数寄存器
#define MONITOR_FILTERA_DDR  	0x0000001c		//监控器过滤器A
#define MONITOR_FILTERB_DDR 	0x0000001e		//监控器过滤器B




volatile unsigned short *ram_cfg_addr			= NULL;

volatile unsigned short *ram_pdate1			= NULL;






volatile unsigned short *control_reg  		= NULL;
volatile unsigned short *status_reg 		= NULL;
volatile unsigned short *command_reg		= NULL;
volatile unsigned short *shortmask_reg  	= NULL;
volatile unsigned short *shortoprate_reg 	= NULL;
volatile unsigned short *shortpnt_reg 		= NULL;
volatile unsigned short *test_reg			= NULL;

volatile unsigned short *bc_fm_rt_tt  		= NULL;
volatile unsigned short *bc_cp_rt_dp 		= NULL;

volatile unsigned short *rt_status 			= NULL;
volatile unsigned short *rt_leg				= NULL;

volatile unsigned short *monitor_cmdpnt  	= NULL;
volatile unsigned short *monitor_blk 		= NULL;
volatile unsigned short *monitor_cnt		= NULL;
volatile unsigned short *monitor_filterA  	= NULL;
volatile unsigned short *monitor_filterB 	= NULL;







//设备号
dev_t dev1553b;
//声明cdevtm
//static cdev dev1553b_cdev;
//定义mutex,互斥锁
struct mutex my_mutex;

//设备类指针
static struct class* dev1553b_class = NULL;
//设备指针
static struct device* dev1553b_device = NULL;


static int dev1553b_open(struct inode * inode , struct file * filp)
{
	int i=0;
	short cfg1[8] = {0x4200,0x0820, 0,0x1000,0,0,0,0};
	short cfg2[8] = {0x4200,0x0c24, 0,0x1020,0,0,0,0};
	short cfg3[8] = {0x4200,0x2860, 0,0x1020,0,0,0,0};
	short cfg4[8] = {0,0,0,0,0,0,0,0};
	
	
	printk("dev1553b_open\n");

	//初始化1553b并设置为bc模式
	//初始化，配置为BC
	
	iowrite16(0x0416,control_reg);
	iowrite16(0x0000,status_reg);
	iowrite16(0xffff,shortmask_reg);
	iowrite16(0xf000,monitor_cnt);
	iowrite16(0x0000,bc_cp_rt_dp);
	
	
	
	//配置命令字	
	for(i=0; i<8; i++)
	{
		*(ram_cfg_addr+i)=*(cfg1+i);
		*(ram_cfg_addr+i+8)=*(cfg2+i);
		*(ram_cfg_addr+i+16)=*(cfg3+i);
		*(ram_cfg_addr+i+24)=*(cfg4+i);
				
	}
	
	
/*	
	*ram_cfg_addr=0x4200;
	*ram_sta_addr=0x0820;
	*ram_pdate_addr = 0x1000;
*/	
	
/*	
	*((unsigned short *)(ram_cfg_addr +0x4))=0;
	*((unsigned short *)(ram_cfg_addr +0x6))=0x1000;
	*((unsigned short *)(ram_cfg_addr +0x8))=0;
	*((unsigned short *)(ram_cfg_addr +0xa))=0;
	*((unsigned short *)(ram_cfg_addr +0xc))=0;
	*((unsigned short *)(ram_cfg_addr +0xe))=0;
	
	
	
	*((unsigned short *)(ram_cfg_addr +0x10))=0x4200;
	*((unsigned short *)(ram_cfg_addr +0x12))=0x0c24;
	*((unsigned short *)(ram_cfg_addr +0x14))=0;
	*((unsigned short *)(ram_cfg_addr +0x16))=0x1020;
	*((unsigned short *)(ram_cfg_addr +0x18))=0;
	*((unsigned short *)(ram_cfg_addr +0x1a))=0;
	*((unsigned short *)(ram_cfg_addr +0x1c))=0;
	*((unsigned short *)(ram_cfg_addr +0x1e))=0;	
	
	
	
	*((unsigned short *)(ram_cfg_addr +0x20))=0;
	*((unsigned short *)(ram_cfg_addr +0x22))=0;
	*((unsigned short *)(ram_cfg_addr +0x24))=0;
	*((unsigned short *)(ram_cfg_addr +0x26))=0;
	*((unsigned short *)(ram_cfg_addr +0x28))=0;
	*((unsigned short *)(ram_cfg_addr +0x2a))=0;
	*((unsigned short *)(ram_cfg_addr +0x2c))=0;
	*((unsigned short *)(ram_cfg_addr +0x2e))=0;			
	
	
	
	//写
	cpu_write_mblk(0x00  ,0x4200,0x0824, 0,0x1000,0,0,0,0); //1
	cpu_write_mblk(0x10 ,0x4200,0x0c24, 0,0x1020,0,0,0,0);//2
	
	cpu_write_mblk(32,0,0,0,0,0,0,0,0);  //写入命令
*/	
	
	

	//*control_reg = 0x8416;
	
	return 0;
}

static ssize_t dev1553b_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	short ret;
	int i=0;

	short val[255] = {0};
	
	//*1553b_CONTROL_REGISTER=0x3800;
	
//    printk("<-1-.drvdev1553b_write=======\n");
	//加锁
	mutex_lock(&my_mutex);

    ret = copy_from_user(&val, buf, count); //copy_to_user();
	printk("dev1553b-ret = -%d-\n",ret);

	for(i=0; i<255; i++)
	{
		printk("kennel-val[%d] = %d\n",i,val[i]);		
	}

	
	for(i=0; i<count/2; i++)
	{
		*(ram_pdate1+i)=*(val+i);
				
	}
	
/*
	*ram_pdate1=*val;
	*(ram_pdate1+0x2)=*(val+1);
	*(ram_pdate1+0x4)=*(val+2);
	*(ram_pdate1+0x6)=*(val+3);
	*(ram_pdate1+0x8)=*(val+4);
	
*/
	
	//printk("dev1553bwrite-ret = -%d- \n",ret);
	
	//printk("kernel-dev1553b_write-ret = %x \n",ret);
	
	//printk("1553b_FIFO %x \n",ioread32(1553b_FIFO));
	mutex_unlock(&my_mutex);
	
	*control_reg = 0x8416;

//	printk("-2->.drvdev1553b_write=======\n");	
		
			
    return 0;
}

static ssize_t dev1553b_read(struct file *file,char __user *buf, size_t count, loff_t * ppos)
{
	short ret,val;
	
	//*1553b_CONTROL_REGISTER=0x3800;

//    printk("<-1-.drvdev1553b_read\n");
	mutex_lock(&my_mutex);

	//if((*1553b_STATUS_REGISTER==0x27a0)||(*1553b_STATUS_REGISTER==0x2fa0)||(*1553b_STATUS_REGISTER==0xfa0))
	//{
		ret = copy_to_user(buf, &val, count);
	//}
	mutex_unlock(&my_mutex);
	

    return 0;
}


//声明操作函数集合
static struct file_operations dev1553b_fops = {
	.owner  =   THIS_MODULE,    //这是一个宏，推向编译模块时自动创建的__this_module变量
	.open   =   dev1553b_open,     
	.write  =   dev1553b_write,
	.read  	=   dev1553b_read,    
};


//加载函数
int ret;
static int __init dev1553b_init(void)
{
	printk("dev1553b_drv_init\n");	
	//1.注册字符设备驱动，告诉内核	
    ret = register_chrdev(0, "dev1553b", &dev1553b_fops);
    if (ret < 0){
        printk("failed to register dev1553b.\n");
        return -1;
    }
	//构建设备号
	dev1553b = MKDEV(ret, MINOR_1553B);
	
	//2.注册设备类
	/*	成功会在/sys/class目录下出现dev1553b子目录*/
    dev1553b_class = class_create(THIS_MODULE, "dev1553b");
    if(IS_ERR(dev1553b_class)){
        printk("failed to create dev1553b moudle class.\n");
        unregister_chrdev(ret, "dev1553b");
        return -1;
    }
	
	
	//3.创建设备文件
	/* dev/dev1553b */;
    dev1553b_device = device_create(dev1553b_class, NULL, dev1553b, NULL, "dev1553b");
    if (IS_ERR(dev1553b_device)){
        printk("failed to create dev1553b .\n");
        unregister_chrdev(ret, "dev1553b");
        return -1;
    }
	
	//4.申请dev1553b0资源，映射
    ram_cfg_addr = (volatile unsigned short *)ioremap(RAM_DDR, 0x1fff);
	
	ram_pdate1 = (volatile unsigned short *)ioremap(RAM_DDR+0x00002000, 0xffff);
	
	
	
	
	
	
	
	control_reg  	= (volatile unsigned short *)ioremap(REG_DDR+CONTROL_REG_DDR,16);
    status_reg 	 	= (volatile unsigned short *)ioremap(REG_DDR+STATUS_REG_DDR,16);
    command_reg		= (volatile unsigned short *)ioremap(REG_DDR+COMMAND_REG_DDR,16);
    shortmask_reg   = (volatile unsigned short *)ioremap(REG_DDR+SHORTMASK_REG_DDR ,16);
    shortoprate_reg = (volatile unsigned short *)ioremap(REG_DDR+SHORTOPRATE_REG_DDR,16);
    shortpnt_reg 	= (volatile unsigned short *)ioremap(REG_DDR+SHORTPNT_REG_DDR,16);
    test_reg		= (volatile unsigned short *)ioremap(REG_DDR+TEST_REG_DDR, 16);
                                                                 
    bc_fm_rt_tt  	= (volatile unsigned short *)ioremap(REG_DDR+BC_FM_RT_TT_DDR,16);
    bc_cp_rt_dp 	= (volatile unsigned short *)ioremap(REG_DDR+BC_CP_RT_DP_DDR,16);
                                                                 
    rt_status 		= (volatile unsigned short *)ioremap(REG_DDR+RT_STATUS_DDR,16);
    rt_leg			= (volatile unsigned short *)ioremap(REG_DDR+RT_LEG_DDR,16);
	                                                             
    monitor_cmdpnt  = (volatile unsigned short *)ioremap(REG_DDR+MONITOR_CMDPNT_DDR,16);
    monitor_blk 	= (volatile unsigned short *)ioremap(REG_DDR+MONITOR_BLK_DDR,16);
    monitor_cnt	    = (volatile unsigned short *)ioremap(REG_DDR+MONITOR_CNT_DDR,16);
    monitor_filterA = (volatile unsigned short *)ioremap(REG_DDR+MONITOR_FILTERA_DDR,16);
    monitor_filterB = (volatile unsigned short *)ioremap(REG_DDR+MONITOR_FILTERB_DDR,16);
	

	//初始化互斥锁
	mutex_init(&my_mutex);
    return 0;
}


//卸载函数
static void __exit dev1553b_exit(void)
{
    printk("1.exit dev1553b module.\n");
	
	//1.解除dev1553b,IO映射
	iounmap(control_reg);
	iounmap(status_reg);
	iounmap(command_reg);
	iounmap(shortmask_reg);
	iounmap(shortoprate_reg);
	iounmap(shortpnt_reg);
	iounmap(test_reg);

	iounmap(bc_fm_rt_tt);
	iounmap(bc_cp_rt_dp);

	iounmap(rt_status);
	iounmap(rt_leg);

	iounmap(monitor_cmdpnt);
	iounmap(monitor_blk);
	iounmap(monitor_cnt);
	iounmap(monitor_filterA);
	iounmap(monitor_filterB);
	
	
	//2.销毁设备文件
    device_destroy(dev1553b_class, dev1553b);
	
	//3.注销设备类
    class_unregister(dev1553b_class);
    class_destroy(dev1553b_class);	
	
	//4.注销字符设备驱动，卸载
    unregister_chrdev(ret, "dev1553b");	
	
    printk("2.dev1553b module exit.\n");	

}

//声明模块出入口
module_init(dev1553b_init);
module_exit(dev1553b_exit);


MODULE_AUTHOR("Qianer");	//作者
MODULE_DESCRIPTION("1553B moudle dirver");	//模块描述
MODULE_VERSION("v1.0");		//版本
MODULE_LICENSE("GPL");		//GPL模块许可证
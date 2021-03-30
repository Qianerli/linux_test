#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <linux/ioctl.h>


//ioctl命令
#define DEV1553B_BC _IO('j',0)
#define DEV1553B_RT _IO('j',1)
#define DEV1553B_BM _IO('j',2)
#define DEV1553B_CFG _IO('j',3)
#define DEV1553B_BM_CS _IO('j',4)

//1553b设备号
#define MINOR_1553B 0


/*---------------------配置1553b寄存器--------------------*/

#define REG_DDR   	0xe0000000 					//1553B寄存器首地址
#define RAM_DDR   	0xe0020000 					//1553B_RAM寄存器首地址

#define RAM_RT   	0x00000042 					//配置为rt时偏移
#define BASEADDR   	0x00002000 					//配置为rt时,初始化时的偏移


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




//1.控制寄存器
volatile unsigned short *control_reg  		= NULL;//0
volatile unsigned short *status_reg 		= NULL;//1
volatile unsigned short *command_reg		= NULL;//2
volatile unsigned short *shortmask_reg  	= NULL;//3
volatile unsigned short *shortoprate_reg 	= NULL;//4
volatile unsigned short *shortpnt_reg 		= NULL;//5
volatile unsigned short *test_reg			= NULL;//6

volatile unsigned short *bc_fm_rt_tt  		= NULL;//7
volatile unsigned short *bc_cp_rt_dp 		= NULL;//8

volatile unsigned short *rt_status 			= NULL;//9
volatile unsigned short *rt_leg				= NULL;//10

volatile unsigned short *monitor_cmdpnt  	= NULL;//11
volatile unsigned short *monitor_blk 		= NULL;//12
volatile unsigned short *monitor_cnt		= NULL;//13
volatile unsigned short *monitor_filterA  	= NULL;//14
volatile unsigned short *monitor_filterB 	= NULL;//15





//2.ram命令字
volatile unsigned short *ram_cfg_addr			= NULL;
//3.ram数据
volatile unsigned short *ram_pdate1			= NULL;



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


//驱动代码返回值和参数值必须给到
void init_rt_ram(void)
{
	volatile unsigned short *addr = NULL;
	
	volatile unsigned short i = 0;
	volatile unsigned short controlword = 0x100;
	volatile unsigned short dateptr1 = 0;
	volatile unsigned short dateptr2 = 0;
	volatile unsigned short dateptr3 = 0;
	
	//地址偏移0x4000
	//ram_cfg_addr+0x2000+4*i+0x00
	addr = ram_cfg_addr+BASEADDR;
	
	//1.Receive
	for(i=0; i<32; i++)
	{
		dateptr1 = i*64;
		dateptr2 = i*64;
		dateptr3 = i*64;
				
		*(addr+4*i+0) = controlword;
		*(addr+4*i+1) = dateptr1;
		*(addr+4*i+2) = dateptr2;
		*(addr+4*i+3) = dateptr3;		
		
	}
	
	//2.Transmit
	for(i=0; i<32; i++)
	{
		dateptr1 = i*64;
		dateptr2 = i*64;
		dateptr3 = i*64;
				
		*(addr+128+4*i+0) = controlword;
		*(addr+128+4*i+1) = dateptr1;
		*(addr+128+4*i+2) = dateptr2;
		*(addr+128+4*i+3) = dateptr3;		
		
	}
	
	//3.Receive
	for(i=0; i<32; i++)
	{
		dateptr1 = 0;
		dateptr2 = 0;
		dateptr3 = 0;
				
		*(addr+256+4*i+0) = controlword;
		*(addr+256+4*i+1) = dateptr1;
		*(addr+256+4*i+2) = dateptr2;
		*(addr+256+4*i+3) = dateptr3;		
		
	}
	
	//4.Transmit
	for(i=0; i<32; i++)
	{
		dateptr1 = 0x7C0;
		dateptr2 = 0x7C0;
		dateptr3 = 0x7C0;
				
		*(addr+384+4*i+0) = controlword;
		*(addr+384+4*i+1) = dateptr1;
		*(addr+384+4*i+2) = dateptr2;
		*(addr+384+4*i+3) = dateptr3;		
		
	}
	
}


static long dev1553b_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	
	short ret;
	int i=0;
	volatile unsigned short *com = NULL;
	volatile unsigned short *date = NULL;
	
	
	short val_bc[5] = {0};
	short val_rt[7] = {0};
	short val_bm[10] = {0};	
	short val_cfg[64] = {0};

	//在内核中还需要将,printk的打印放在变量定义之后
	printk("enter dev1553b_ioctl!\n");	
		
	switch(cmd){
		//初始化1553b并设置为bc模式
		//初始化，配置为BC
		case DEV1553B_BC:
			printk("DEV1553B_BC!\n");

			ret = copy_from_user(&val_bc, (const char*)arg, sizeof(val_bc)); //copy_to_user();
			printk("dev1553b_ioctl_bc-ret = -%d-\n",ret);

			for(i=0; i<5; i++)
			{
				printk("kennel-val_bc[%d] = %x\n",i,*(val_bc+i));		
			}


			*control_reg 	= *val_bc;
			*status_reg   	= *(val_bc+1);
			*shortmask_reg 	= *(val_bc+2);
			*monitor_cnt   	= *(val_bc+3);
			*bc_cp_rt_dp   	= *(val_bc+4);	
			
			
			printk("*control_reg = %x\n",*control_reg);	
			printk("*status_reg = %x\n",*status_reg);	
			printk("*shortmask_reg = %x\n",*shortmask_reg);	
			printk("*monitor_cnt = %x\n",*monitor_cnt);	
			printk("*bc_cp_rt_dp = %x\n",*bc_cp_rt_dp);	
			
			
			
			printk(">===========ioctl_bc_end!=============<\n");
			
			break;
			
		//初始化1553b并设置为RT模式
		//初始化，配置为RT
		case DEV1553B_RT:

			printk("DEV1553B_RT!\n");
			
			//初始化ram
			init_rt_ram();

			ret = copy_from_user(&val_rt, (const char*)arg, sizeof(val_rt)); //copy_to_user();
			printk("dev1553b_ioctl_rt-ret = -%d-\n",ret);

			for(i=0; i<7; i++)
			{
				printk("kennel-val_rt[%d] = %x\n",i,*(val_rt+i));		
			}

			*control_reg		= *val_rt;
			*status_reg			= *(val_rt+1);
			*shortmask_reg		= *(val_rt+2);
			*shortpnt_reg		= *(val_rt+3);
			*bc_fm_rt_tt		= *(val_rt+4);
			*bc_cp_rt_dp 		= *(val_rt+5);
			*rt_status 			= *(val_rt+6);
			
			
			//启动rt
			*control_reg = 0x9d17;
			
			printk(">===========ioctl_rt_end!=============<\n");
			
			break;

		//初始化1553b并设置为BM模式
		//初始化，配置为BM
		case DEV1553B_BM:

			printk("DEV1553B_BM!\n");

			ret = copy_from_user(&val_bm, (const char*)arg, sizeof(val_bm)); //copy_to_user();
			printk("dev1553b_ioctl_bm-ret = -%d-\n",ret);

			for(i=0; i<10; i++)
			{
				printk("kennel-val_bm[%d] = %x\n",i,*(val_bm+i));		
			}

			*control_reg     = *val_bm;
			*status_reg      = *(val_bm+1);
			*shortmask_reg   = *(val_bm+2);
			*shortpnt_reg	 = *(val_bm+3);
			*bc_fm_rt_tt	 = *(val_bm+4);
			*monitor_cmdpnt  = *(val_bm+5);
			*monitor_blk 	 = *(val_bm+6);
			*monitor_cnt     = *(val_bm+7);
			*monitor_filterA = *(val_bm+8);
			*monitor_filterB = *(val_bm+9);	

			//启动bm
			*control_reg = 0x8412;
						
			printk(">===========ioctl_bm_end!=============<\n");
			
			break;


		//写入命令字
		case DEV1553B_CFG:
			printk("DEV1553B_CFG!\n");

			ret = copy_from_user(&val_cfg, (const char*)arg, sizeof(val_cfg)); //copy_to_user();
			printk("dev1553b_ioctl_cfg-ret = -%d-\n",ret);

			for(i=0; i<8; i++)
			{
				printk("kennel-val_cfg[%d] = %x\n",i,*(val_cfg+i));		
			}
			
			printk("sizeof(val_cfg) = %d\n",sizeof(val_cfg));

			//配置命令字	
			for(i=0; i<sizeof(val_cfg); i++)
			{
				*(ram_cfg_addr+i)=*(val_cfg+i);				
			}

					
			printk(">===========ioctl_cfg_end!=============<\n");
			
			break;			


		//测试BM
		case DEV1553B_BM_CS:
			
			printk("DEV1553B_BM_CS!\n");
			
			com = ram_cfg_addr;
			date = ram_cfg_addr+0x1000;
			
			
			//命令字寄存器打印
			for(i=0; i<8*10; i++)
			{				
			printk("0xe0020000-kennel-bm-com-cs[%d] = %x\n",i,*(com+i));
				
			}
			
			
			//数据寄存器打印
			for(i=0; i<8*10; i++)
			{				
				printk("0xe0022000-kennel-bm-date-cs[%d] = %x\n",i,*(date+i));
				
			}			

					
			printk(">===========ioctl_bm_end!=============<\n");
			
			break;
		default:
			return -EINVAL;
	}	
	
	
	return 0;
}


static int dev1553b_open(struct inode * inode , struct file * filp)
{	
	printk("dev1553b_open\n");
	
	return 0;
}


static ssize_t dev1553b_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	short ret;
	int i=0;

	short val[255] = {0};
	
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
	
	mutex_unlock(&my_mutex);
	
	//启动bc
	*control_reg = 0x8416;
		
			
    return 0;
}

static ssize_t dev1553b_read(struct file *file,char __user *buf, size_t count, loff_t * ppos)
{
	volatile unsigned short *val = NULL;
	short ret,i;
	short val_buf[512];
	
	val = ram_cfg_addr+RAM_RT;
	

	for(i=0; i<0x20*3; i++)
	{
		if((i!=0) & !(i%0x20)){val=val+0x20;}
		val_buf[i] = *(val+i);
	
	}	

	//互斥锁
	mutex_lock(&my_mutex);
		
	ret = copy_to_user(buf, &val_buf, count);

	mutex_unlock(&my_mutex);
	

    return 0;
}

int dev1553b_release(struct inode *inode, struct file *filp)
{
	printk("enter dev1553b_release!\n");

	return 0;
}


//声明操作函数集合
static struct file_operations dev1553b_fops = {
	.owner  =   THIS_MODULE,    //这是一个宏，推向编译模块时自动创建的__this_module变量
	.open   =   dev1553b_open,     
	.write  =   dev1553b_write,
	.read  	=   dev1553b_read, 
	.unlocked_ioctl = dev1553b_ioctl,//ioctl接口
	.release = dev1553b_release,//对应用户close接口	
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
	iounmap(ram_cfg_addr);
	iounmap(ram_pdate1);
	
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
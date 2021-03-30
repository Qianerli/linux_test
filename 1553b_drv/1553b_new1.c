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

#define REG_DDR   			0x60000000 			//1553B寄存器首地址
#define RAM_COMMAND_DDR   	0x60020000 			//1553B_RAM命令字寄存器首地址
#define RAM_DATE_DDR   		0x60022000 			//1553B_RAM寄存器首地址


//配置模式寄存器
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



//命令寄存器
#define	CONTROL_WORD_DDR 			0x00000000		//控制字
#define COMMAND_WORD_1_DDR 			0x00000002		//命令字1
#define COMMAND_WORD_2_DDR			0x00000004		//命令字2
#define DATA_POINTER_DDR  			0x00000006		//数据指针
#define STATUS_WORD_1_DDR 			0x00000008		//状态词1
#define STATUS_WORD_2_DDR 			0x0000000a		//状态词2
#define RANCH_ADDRESS_DDR			0x0000000c		//分支地址
#define TIMER_VALUE_DDR				0x0000000e		//计时器值


/************************************************************/

//配置模式寄存器指针
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




//命令寄存器指针
volatile unsigned short *control_word 		= NULL;
volatile unsigned short *command_word_1 	= NULL;
volatile unsigned short *command_word_2		= NULL;
volatile unsigned short *data_pointer  		= NULL;
volatile unsigned short *status_word_1 	 	= NULL;
volatile unsigned short *status_word_2 		= NULL;
volatile unsigned short *ranch_address		= NULL;
volatile unsigned short *timer_value		= NULL;

volatile unsigned short *control_word_2 		= NULL;
volatile unsigned short *command_word_1_2 		= NULL;
volatile unsigned short *command_word_2_2		= NULL;
volatile unsigned short *data_pointer_2  		= NULL;
volatile unsigned short *status_word_1_2 	 	= NULL;
volatile unsigned short *status_word_2_2 		= NULL;
volatile unsigned short *ranch_address_2		= NULL;
volatile unsigned short *timer_value_2			= NULL;


//ramdate寄存器映射
volatile unsigned short *ramdate			= NULL;



/************************************************************/









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


/*
void cpu_write_mblk(unsigned short a,unsigned short b,unsigned short c,\
unsigned short d,unsigned short e,unsigned short f,unsigned short g,\
unsigned short h,unsigned short i)
{

	*((unsigned short *)(ram_ddr + a))=b;
	*((unsigned short *)(ram_ddr + a+0x2))=c;
	*((unsigned short *)(ram_ddr + a+0x4))=d;
	*((unsigned short *)(ram_ddr + a+0x6))=e;
	*((unsigned short *)(ram_ddr + a+0x8))=f;
	*((unsigned short *)(ram_ddr + a+0xa))=g;
	*((unsigned short *)(ram_ddr + a+0xc))=h;
	*((unsigned short *)(ram_ddr + a+0xe))=i;

	
	iowrite16(b,ram_ddr+a);
	iowrite16(c,ram_ddr+a+0x2);
	iowrite16(d,ram_ddr+a+0x4);
	iowrite16(e,ram_ddr+a+0x6);
	iowrite16(f,ram_ddr+a+0x8);
	iowrite16(g,ram_ddr+a+0xa);
	iowrite16(h,ram_ddr+a+0xc);
	iowrite16(i,ram_ddr+a+0xe);

}

*/



void Command_cfg_date(unsigned short control_date,unsigned short command_date_1,unsigned short command_date_2,\
unsigned short data_date,unsigned short status_date_1,unsigned short status_date_2,unsigned short ranch_date,\
unsigned short timer_date)
{

	iowrite16(control_date,control_word);
	iowrite16(command_date_1,command_word_1);
	iowrite16(command_date_2,command_word_2);
	iowrite16(data_date,data_pointer);
	iowrite16(status_date_1,status_word_1);
	iowrite16(status_date_2,status_word_2);
	iowrite16(ranch_date,ranch_address);
	iowrite16(timer_date,timer_value);
	
}


void Command_cfg_end(unsigned short control_date,unsigned short command_date_1,unsigned short command_date_2,\
unsigned short data_date,unsigned short status_date_1,unsigned short status_date_2,unsigned short ranch_date,\
unsigned short timer_date)
{

	iowrite16(control_date,control_word_2);
	iowrite16(command_date_1,command_word_1_2);
	iowrite16(command_date_2,command_word_2_2);
	iowrite16(data_date,data_pointer_2);
	iowrite16(status_date_1,status_word_1_2);
	iowrite16(status_date_2,status_word_2_2);
	iowrite16(ranch_date,ranch_address_2);
	iowrite16(timer_date,timer_value_2);
	
}



static int dev1553b_open(struct inode * inode , struct file * filp)
{
	printk("dev1553b_open\n");

	//初始化1553b并设置为bc模式
	//初始化，配置模式寄存器，配置为BC
	
	iowrite16(0x0416,control_reg);
	iowrite16(0x0000,status_reg);
	iowrite16(0xffff,shortmask_reg);
	iowrite16(0xf000,monitor_cnt);
	iowrite16(0x0000,bc_cp_rt_dp);
	
	
	
/*	
	*control_reg=0x0416;//0
	*status_reg=0x0000;//1
	*shortmask_reg=0xffff;//3
	*monitor_cnt=0xf000;//5
	*bc_cp_rt_dp = 0x0000;//8
*/
	
	printk("control_reg %x \n",ioread16(control_reg));
	printk("status_reg %x \n",ioread16(status_reg));
	printk("shortmask_reg %x \n",ioread16(shortmask_reg));
	printk("monitor_cnt %x \n",ioread16(monitor_cnt));
	printk("bc_cp_rt_dp %x \n",ioread16(bc_cp_rt_dp));
	
	
	//配置命令字
	//写
	Command_cfg_date(0x4200,0x0824, 0,0x1000,0,0,0,0); //1
	Command_cfg_end(0,0,0,0,0,0,0,0);  //写入命令

	//*control_reg = 0x8416;
	
	return 0;
}

static ssize_t dev1553b_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	short ret,val;

	
	//*1553b_CONTROL_REGISTER=0x3800;
	
//    printk("<-1-.drvdev1553b_write=======\n");
	//加锁
	mutex_lock(&my_mutex);

    ret = copy_from_user(&val, buf, count); //copy_to_user();
//	printk("dev1553b-ret = -%d-\n",ret);
	
	*ramdate=val;
	
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
	
	//配置模式寄存器映射
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
	
	
	//命令字寄存器映射
	control_word 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+CONTROL_WORD_DDR,16);
    command_word_1 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+COMMAND_WORD_1_DDR,16);
    command_word_2	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+COMMAND_WORD_2_DDR,16);
    data_pointer  	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+DATA_POINTER_DDR ,16);
    status_word_1 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+STATUS_WORD_1_DDR,16);
    status_word_2 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+STATUS_WORD_2_DDR,16);
    ranch_address	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+RANCH_ADDRESS_DDR, 16);                                                                
    timer_value		= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+TIMER_VALUE_DDR,16);
	
	
	control_word_2 		= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+CONTROL_WORD_DDR+0x10,16);
    command_word_1_2 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+COMMAND_WORD_1_DDR+0x10,16);
    command_word_2_2	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+COMMAND_WORD_2_DDR+0x10,16);
    data_pointer_2  	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+DATA_POINTER_DDR+0x10 ,16);
    status_word_1_2 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+STATUS_WORD_1_DDR+0x10,16);
    status_word_2_2 	= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+STATUS_WORD_2_DDR+0x10,16);
    ranch_address_2		= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+RANCH_ADDRESS_DDR+0x10, 16);                                                                
    timer_value_2		= (volatile unsigned short *)ioremap(RAM_COMMAND_DDR+TIMER_VALUE_DDR+0x10,16);
	
	//ramdate寄存器映射
	ramdate			= (volatile unsigned short *)ioremap(RAM_DATE_DDR,0xffff);
	
	
	
	
	
	
	

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
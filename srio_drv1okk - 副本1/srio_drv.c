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

#define SRIO_MINOR 0

//ioctl命令
#define SRIO_NWRITE _IO('s',0)
#define SRIO_NREAD _IO('s',1)
#define SRIO_DOORBELL _IO('s',2)
#define SRIO_NREAD_SIZE _IO('s',3)


/*---------------------配置srio寄存器--------------------*/


#define IRQ_BASE_DDR       	0x80000200
#define TGT_CMD_BASE_DDR   	0x80020000  // target_reg
#define INIT_CMD_BASE_DDR  	0x80030000  // initiator_reg

#define INIT_DATA_BASE_DDR 	0xfff00000  // initiator AXI_FIFO axi_full 
#define INIT_REG_BASE_DDR  	0xfff02000  // initiator AXI_FIFO axi_lite

#define TGT_DATA_BASE_DDR  	0xfff04000  // target AXI_FIFO axi_full
#define TGT_REG_BASE_DDR   	0xfff06000  // target AXI_FIFO axi_lite


//偏移地址
#define IRQ_PEND_DDR  		0x00000004
#define IRQ_CLEAR_DDR 		0x0000000C

#define TARGET_REG0_DDR 	0x00000000
#define TARGET_REG1_DDR 	0x00000004
#define LINK_STATE_DDR  	0x00000008

//3012
#define INITIATOR_REG0_DDR 	0x00000000	
#define INITIATOR_REG1_DDR 	0x00000004	
#define INITIATOR_REG2_DDR 	0x00000008	
#define INITIATOR_REG3_DDR 	0x0000000c	

#define INIT_TGT_ISR_DDR   	0x00000000
#define INIT_TGT_IER_DDR   	0x00000004
#define INIT_TGT_TDFR_DDR  	0x00000008
#define INIT_TGT_TDFV_DDR  	0x0000000C
#define INIT_TGT_TLR_DDR   	0x00000014
#define INIT_TGT_RDFR_DDR  	0x00000018
#define INIT_TGT_RDFO_DDR  	0x0000001C
#define INIT_TGT_RLR_DDR   	0x00000024
#define INIT_TGT_SRR_DDR   	0x00000028
#define INIT_TGT_TDR_DDR   	0x0000002C
#define INIT_TGT_RDR_DDR   	0x00000030

#define INIT_TGT_TDFD_DDR  	0x00000000
#define INIT_TGT_RDFD_DDR  	0x00001000



volatile unsigned int *irq_pend  	= NULL;
volatile unsigned int *irq_clear 	= NULL;
	
volatile unsigned int *target_reg0 	= NULL;
volatile unsigned int *target_reg1 	= NULL;
volatile unsigned int *link_state  	= NULL;


//定义事务//可写不可读
volatile unsigned int *initiator_reg0 	= NULL;
volatile unsigned int *initiator_reg1 	= NULL;
volatile unsigned int *initiator_reg2 	= NULL;
volatile unsigned int *initiator_reg3 	= NULL;


volatile unsigned long long *init_tdfd  = NULL;
volatile unsigned long long *init_rdfd  = NULL;
		
volatile unsigned int *init_isr   = NULL;
volatile unsigned int *init_ier   = NULL;
volatile unsigned int *init_tdfr  = NULL;
volatile unsigned int *init_tdfv  = NULL;
volatile unsigned int *init_tlr   = NULL;
volatile unsigned int *init_rdfr  = NULL;
volatile unsigned int *init_rdfo  = NULL;
volatile unsigned int *init_rlr   = NULL;
volatile unsigned int *init_srr   = NULL;
volatile unsigned int *init_tdr   = NULL;
volatile unsigned int *init_rdr   = NULL;
		
volatile unsigned long long *tgt_tdfd  = NULL;
volatile unsigned long long *tgt_rdfd  = NULL;
		
		
volatile unsigned int *tgt_isr   = NULL;
volatile unsigned int *tgt_ier   = NULL;
volatile unsigned int *tgt_tdfr  = NULL;
volatile unsigned int *tgt_tdfv  = NULL;
volatile unsigned int *tgt_tlr   = NULL;
volatile unsigned int *tgt_rdfr  = NULL;
volatile unsigned int *tgt_rdfo  = NULL;
volatile unsigned int *tgt_rlr   = NULL;
volatile unsigned int *tgt_srr   = NULL;
volatile unsigned int *tgt_tdr   = NULL;
volatile unsigned int *tgt_rdr   = NULL;


//设备号
dev_t devsrio;
//声明cdevtm
//static cdev srio_cdev;
//定义mutex,互斥锁
struct mutex my_mutex;

//设备类指针
static struct class* srio_class = NULL;
//设备指针
static struct device* srio_device = NULL;

static unsigned int val_nread[4] = {0};
static unsigned int nwrite_size = 0;


static long srio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret,i=0;
	
	
	//在内核中还需要将,printk的打印放在变量定义之后
	printk("enter srio_ioctl!\n");
	
	switch(cmd){
		//初始化srio并设置为NWRITE模式
		//初始化，配置为NWRITE
		case SRIO_NWRITE:
			printk("SRIO_NWRITE!\n");

			ret = copy_from_user(&val_nread, (const char*)arg, sizeof(val_nread)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);

			for(i=0; i<4; i++)
			{
				printk("kennel-val_nread[%d] = %x\n",i,*(val_nread+i));		
			}


			*initiator_reg0	= *val_nread;
			*initiator_reg1	= *(val_nread+1);
			*initiator_reg2 = *(val_nread+2);
			*initiator_reg0 = *(val_nread+3);	
						
			
			printk("initiator NWRITE_R request:\n");  
			printk("***** board A ****\n"); 
			printk("init_isr :  0x%x\n", ioread32(init_isr));
			
			
			printk(">===========ioctl_nwrite_end!=============<\n");
			
			break;
		//初始化srio并设置为NREAD模式
		//初始化，配置为NREAD
		case SRIO_NREAD:
			printk("SRIO_NREAD!\n");

			ret = copy_from_user(&val_nread, (const char*)arg, sizeof(val_nread)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);

			for(i=0; i<4; i++)
			{
				printk("kennel-val_nread[%d] = %x\n",i,*(val_nread+i));		
			}


			*initiator_reg0	= *val_nread;
			*initiator_reg1	= *(val_nread+1);
			*initiator_reg2 = *(val_nread+2);
			*initiator_reg0 = *(val_nread+3);	
						
			
			printk("initiator NREAD request:\n");  
			printk("***** board A ****\n"); 
			printk("init_isr :  0x%x\n", ioread32(init_isr));
			
			
			printk(">===========ioctl_nread_end!=============<\n");
			
			break;
		//初始化srio并设置为DOORBELL模式
		//初始化，配置为DOORBELL
		case SRIO_DOORBELL:
			printk("SRIO_DOORBELL!\n");

			ret = copy_from_user(&val_nread, (const char*)arg, sizeof(val_nread)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);

			for(i=0; i<4; i++)
			{
				printk("kennel-val_doorbell[%d] = %x\n",i,*(val_nread+i));		
			}


			*initiator_reg0	= *val_nread;
			*initiator_reg1	= *(val_nread+1);
			*initiator_reg2 = *(val_nread+2);
			*initiator_reg0 = *(val_nread+3);	
						
			
			printk("initiator SRIO_DOORBELL request:\n");  
			printk("init_isr :  0x%x\n", ioread32(init_isr));
				
			printk(">===========ioctl_doorbell_end!=============<\n");
			
			
			printk("*target_reg1 = %x\n"	,ioread16(target_reg1));
			
			break;
		//当在NREAD模式下时write获取到size
		case SRIO_NREAD_SIZE:
			printk("SRIO_NREAD_SIZE!\n");

			nwrite_size = ((unsigned int)(*target_reg1 & 0x00ff0000) >> 16)+1;
			
			//将size传输到应用层，给target使用
			ret = copy_to_user((unsigned char *)arg, &nwrite_size, sizeof(nwrite_size)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);


			printk("kennel-nwrite_size = %x\n",nwrite_size);	


				
			printk(">===========ioctl_nwrite_size_end!=============<\n");
			
			
			break;
		default:
			return -EINVAL;
	}	
	
	return 0;	
}


static int srio_open(struct inode * inode , struct file * filp)
{	
	
	printk("drvsrio_open\n");
	
	*init_ier=0xffffffff;
	*init_isr=0xffffffff;
	printk("init fifo is initilized\n");	
	
	*tgt_ier=0xffffffff;	
	*tgt_isr=0xffffffff;
	printk("tgt fifo is initilized\n");	
	
	*irq_clear = 0x1080; //清除irq_pend的[7]和[12]
	printk("irq is cleared\n\n");	
	
	printk("link_state: 0x%p: 0x%x\n", link_state, *link_state);
	if(*link_state & 0x00000001) 
		  printk("link up!\n\n");		
	else 
			printk("link down!\n\n");
  

	return 0;
}

static ssize_t srio_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	
	int ret,i;
	unsigned long long val[128] = {0};

	printk("srio_write\n");
	
	printk("*irq_pend = %x\n"	,*irq_pend);

	if(*val_nread==5)
	{
		printk("(*target_reg0 & 0x3c000000)/4000000 = %x\n"	,(*target_reg0 & 0x3c000000)/4000000);
		//nwrite事务的写
		mutex_lock(&my_mutex);

		ret = copy_from_user(&val, buf, count); //copy_to_user();
		printk("srio-nwriter-ret = -%d-\n",ret);

		
		for(i=0; i<count/8; i++)
		{
			printk("kennel-write-val[%d] = %llx\n",i,val[i]);		
		}	


		for(i=0; i<count/8; i++)
		{
			*init_tdfd=*(val+i);
					
		}
		//写入数据大小		
		printk("init_tdfv: 0x%x\n", ioread32(init_tdfv));		
		printk("init_isr : 0x%x\n", ioread32(init_isr));	

		*init_tlr=count;

		//清理中断	
		printk("init_tdfv:0x%x\n",  ioread32(init_tdfv));			
		printk("init_isr :0x%x\n", ioread32(init_isr));
		
		*init_isr=0xffffffff;
		
		printk("init_isr :0x%x\n\n", ioread32(init_isr));

		mutex_unlock(&my_mutex);
		
	}
	else if((*target_reg0 & 0x3c000000)/0x4000000==2)
	{
		while(!(*irq_pend & 0x1000));//irq_pend[12]=1，即target收到NREAD请求事务后产生了一个中断		
		printk("*target_reg0  = %x\n"	,*target_reg0);
		printk("*target_reg1  = %x\n"	,*target_reg1);
		printk("(*target_reg1 & 0x00ff0000) >> 16 = %x\n"	,(*target_reg1 & 0x00ff0000) >> 16);
		printk("*target_reg0 & 0x3c000000)/4000000 = %x\n"	,(*target_reg0 & 0x3c000000)/0x4000000);
		printk("(((*target_reg1 & 0x00ff0000)/0x10000)+1)/8 = %x\n"	,(((*target_reg1 & 0x00ff0000)/0x10000)+1)/8);
		//nread事务的写

		printk("(*target_reg0 & 0x3c000000)/4000000 = %x\n"	,(*target_reg0 & 0x3c000000)/4000000);
		//nread事务的写
		mutex_lock(&my_mutex);

		ret = copy_from_user(&val, buf, count); //copy_to_user();
		printk("srio-ret = -%d-\n",ret);

		
		for(i=0; i<count/8; i++)
		{
			printk("kennel-write-val[%d] = %llx\n",i,val[i]);		
		}	

		for(i=0; i<count/8; i++)
		{
			*tgt_tdfd=*(val+i);
					
		}
		//写入数据大小		
	
		*tgt_tlr = count; //32*8byte=256byte

		//清理中断	
		
		*irq_clear = 0x1000;
		
		//printk("irq_clear :0x%x\n\n", ioread32(irq_clear));
		printk(">=============write================<\n");

		mutex_unlock(&my_mutex);	
		
	}	

	
    return 0;
}

static ssize_t srio_read(struct file *file,char __user *buf, size_t count, loff_t * ppos)
{
	
	volatile unsigned long long *val = NULL;
	int ret,i;
	unsigned long long val_buf[128] = { 0 };

	printk("srio_read\n");
	
	if((*target_reg0 & 0x3c000000)/0x4000000==5)
	{
		printk("val_nread[3]: 0x%x\n", val_nread[3]);
		val = tgt_rdfd;	
		printk("tgt_isr : 0x%x\n", *tgt_isr);

		//堵塞中断并等待
		while(!(*irq_pend & 0x00001000));
		
		
		//(*tgt_rlr/8)
		for(i=0; i<count/8; i++)
		{
			val_buf[i] = (unsigned long long)*val;
			printk("kennel-read-val_buf[%d] = %llx\n",i,val_buf[i]);	
		
		}

		printk("target_reg0: 0x%x\n", *target_reg0);
		printk("target_reg1: 0x%x\n", *target_reg1);
		

		mutex_lock(&my_mutex);

		ret = copy_to_user(buf, &val_buf, count);

		mutex_unlock(&my_mutex);

		//清理中断
		*irq_clear = 0x0000fffe;
		printk("irq_pend:0x%x\n",*irq_pend);	
		
		

	}
	else if(val_nread[3]==0x80000002)
	{	
		printk("val_nread[3]: 0x%x\n", val_nread[3]);

		printk("*irq_pend = %x\n"	,*irq_pend);

		val = init_rdfd;	

		//堵塞中断并等待
		while(!(*irq_pend & 0x80)); //irq_pend[7]=1,即initiator收到NREAD响应事务后产生了一个中断
		printk("initiator generates an irq\n");	
		while(!(*init_isr & 0x4000000)); //检测initiator FIFO是否收到NREAD响应事务的数据		
		
		
		//(*tgt_rlr/8)
		for(i=0; i<count/8; i++)
		{
			val_buf[i] = (unsigned long long)*val;
			printk("kennel-read-val_buf[%d] = %llx\n",i,val_buf[i]);	
		
		}

		printk("target_reg0: 0x%x\n", *target_reg0);
		printk("target_reg1: 0x%x\n", *target_reg1);
		

		mutex_lock(&my_mutex);

		ret = copy_to_user(buf, &val_buf, count);

		mutex_unlock(&my_mutex);

		//清理中断
		*irq_clear = 0x80; //清除中断irq_pend[7]
		*init_isr = 0xffffffff; //清除initiator FIFO的状态
		printk("irq_pend:0x%x\n",*irq_pend);
		
		
	}

    return 0;
}

int srio_release(struct inode *inode, struct file *filp)
{
	printk("enter srio_release!\n");
	
	return 0;
}


//声明操作函数集合
static struct file_operations srio_fops = {
	.owner  =   THIS_MODULE,    //这是一个宏，推向编译模块时自动创建的__this_module变量
	.open   =   srio_open,     
	.write  =   srio_write,
	.read  	=   srio_read,
	.unlocked_ioctl = srio_ioctl,//ioctl接口
	.release = srio_release,//对应用户close接口		
};


//加载函数
int ret;
static int __init srio_init(void)
{
	printk("srio_drvtm_init\n");	
	//1.注册字符设备驱动，告诉内核	
    ret = register_chrdev(0, "srio", &srio_fops);
    if (ret < 0){
        printk("failed to register devsrio.\n");
        return -1;
    }
	//构建设备号
	devsrio = MKDEV(ret, SRIO_MINOR);
	
	//2.注册设备类
	/*	成功会在/sys/class目录下出现srio子目录*/
    srio_class = class_create(THIS_MODULE, "srio");
    if(IS_ERR(srio_class)){
        printk("failed to create srio moudle class.\n");
        unregister_chrdev(ret, "srio");
        return -1;
    }
	
	
	//3.创建设备文件
	/* dev/srio */;
    srio_device = device_create(srio_class, NULL, devsrio, NULL, "srio");
    if (IS_ERR(srio_device)){
        printk("failed to create devsrio .\n");
        unregister_chrdev(ret, "srio");
        return -1;
    }
	
	//4.申请srio资源，映射
	irq_pend  	  	= (volatile unsigned int *)ioremap(IRQ_BASE_DDR+IRQ_PEND_DDR, 32);
	irq_clear 	  	= (volatile unsigned int *)ioremap(IRQ_BASE_DDR+IRQ_CLEAR_DDR, 32);

	target_reg0 	= (volatile unsigned int *)ioremap(TGT_CMD_BASE_DDR+TARGET_REG0_DDR, 32);
	target_reg1 	= (volatile unsigned int *)ioremap(TGT_CMD_BASE_DDR+TARGET_REG1_DDR, 32);
	link_state  	= (volatile unsigned int *)ioremap(TGT_CMD_BASE_DDR+LINK_STATE_DDR , 32);

	initiator_reg0 	= (volatile unsigned int *)ioremap(INIT_CMD_BASE_DDR+INITIATOR_REG0_DDR	, 32);
	initiator_reg1 	= (volatile unsigned int *)ioremap(INIT_CMD_BASE_DDR+INITIATOR_REG1_DDR	, 32);
	initiator_reg2 	= (volatile unsigned int *)ioremap(INIT_CMD_BASE_DDR+INITIATOR_REG2_DDR , 32);
	initiator_reg3 	= (volatile unsigned int *)ioremap(INIT_CMD_BASE_DDR+INITIATOR_REG3_DDR	, 32);

	init_tdfd  		= (volatile unsigned long long *)ioremap(INIT_DATA_BASE_DDR+INIT_TGT_TDFD_DDR, 0xfff);
	init_rdfd  		= (volatile unsigned long long *)ioremap(INIT_DATA_BASE_DDR+INIT_TGT_RDFD_DDR, 0xfff);

	init_isr   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_ISR_DDR  , 32);
	init_ier   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_IER_DDR  , 32);
	init_tdfr  		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_TDFR_DDR , 32);
	init_tdfv  		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_TDFV_DDR , 32);
	init_tlr   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_TLR_DDR  , 32);
	init_rdfr  		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_RDFR_DDR , 32);
	init_rdfo  		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_RDFO_DDR , 32);
	init_rlr   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_RLR_DDR  , 32);
	init_srr   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_SRR_DDR  , 32);
	init_tdr   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_TDR_DDR  , 32);
	init_rdr   		= (volatile unsigned int *)ioremap(INIT_REG_BASE_DDR+INIT_TGT_RDR_DDR  , 32);

	tgt_tdfd  		= (volatile unsigned long long *)ioremap(TGT_DATA_BASE_DDR+INIT_TGT_TDFD_DDR, 0xfff);
	tgt_rdfd  		= (volatile unsigned long long *)ioremap(TGT_DATA_BASE_DDR+INIT_TGT_RDFD_DDR, 0xfff);

	tgt_isr   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_ISR_DDR  , 32);
	tgt_ier   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_IER_DDR  , 32);
	tgt_tdfr  		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_TDFR_DDR , 32);
	tgt_tdfv  		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_TDFV_DDR , 32);
	tgt_tlr   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_TLR_DDR  , 32);
	tgt_rdfr  		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_RDFR_DDR , 32);
	tgt_rdfo  		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_RDFO_DDR , 32);
	tgt_rlr   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_RLR_DDR  , 32);
	tgt_srr   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_SRR_DDR  , 32);
	tgt_tdr   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_TDR_DDR  , 32);
	tgt_rdr   		= (volatile unsigned int *)ioremap(TGT_REG_BASE_DDR+INIT_TGT_RDR_DDR  , 32);


	//初始化互斥锁
	mutex_init(&my_mutex);
    return 0;
}


//卸载函数
static void __exit srio_exit(void)
{
    printk("1.exit srio module.\n");
	
	//1.解除srio,IO映射
	iounmap(irq_pend);
	iounmap(irq_clear);

	iounmap(target_reg0);
	iounmap(target_reg1);
    iounmap(link_state);

	iounmap(initiator_reg0);
    iounmap(initiator_reg1);
	iounmap(initiator_reg2);
	iounmap(initiator_reg3);

	iounmap(init_tdfd);
	iounmap(init_rdfd);

	iounmap(init_isr);
	iounmap(init_ier);
    iounmap(init_tdfr);
	iounmap(init_tdfv);
	iounmap(init_tlr);
    iounmap(init_rdfr);
	iounmap(init_rdfo);
	iounmap(init_rlr);
    iounmap(init_srr);
	iounmap(init_tdr);
	iounmap(init_rdr);

	iounmap(tgt_tdfd);
	iounmap(tgt_rdfd);

	iounmap(tgt_isr);
	iounmap(tgt_ier);
    iounmap(tgt_tdfr);
	iounmap(tgt_tdfv);
	iounmap(tgt_tlr);
    iounmap(tgt_rdfr);
	iounmap(tgt_rdfo);
	iounmap(tgt_rlr);
    iounmap(tgt_srr);
	iounmap(tgt_tdr);
	iounmap(tgt_rdr);


	//2.销毁设备文件
    device_destroy(srio_class, devsrio);
	
	//3.注销设备类
    class_unregister(srio_class);
    class_destroy(srio_class);	
	
	//4.注销字符设备驱动，卸载
    unregister_chrdev(ret, "srio");	
	
    printk("2.srio module exit.\n");	

}

//声明模块出入口
module_init(srio_init);
module_exit(srio_exit);


MODULE_AUTHOR("Qianer");	//作者
MODULE_DESCRIPTION("SRIO moudle dirver");	//模块描述
MODULE_VERSION("v1.0");		//版本
MODULE_LICENSE("GPL");		//GPL模块许可证
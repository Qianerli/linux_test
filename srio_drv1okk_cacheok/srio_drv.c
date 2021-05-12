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
#include <linux/dma-mapping.h>

#include <asm/string.h>
#include <linux/string.h>

//#include <asm/cachetlb_32.h>
#include <asm/cacheflush.h>



#define SRIO_MINOR 0

//ioctl命令
#define SRIO_DMA _IO('s',0)
#define SRIO_NWRITE _IO('s',1)
#define SRIO_DOORBELL _IO('s',2)
#define SRIO_DMA_DDR _IO('s',3)
#define SRIO_DOORBELL_DATE _IO('s',4)     
#define SRIO_FLUSH_DMA _IO('s',5) 
#define SRIO_LEON_FLUSH_DMA _IO('s',6) 

//申请DMA空间2MB
#define COPY_BUFF_SIZE (1024*8)*2

static unsigned long long *src_addr;              //源虚拟地址
static dma_addr_t src_phy_addr;    	//源物理地址


/*---------------------配置srio寄存器--------------------*/


#define IRQ_PEND_BASE_DDR   	0x80000204  // target_reg
#define IRQ_CLEAR_BASE_DDR   	0x8000020C  // target_reg

#define TGT_INIT_BASE_DDR   	0x80020000  // target_reg

#define L2CACHE_BASE_DDR   		0xff000000 // l2cache


#define INIT_REG0_DDR   		0x00000000
#define INIT_REG1_DDR   		0x00000004
#define INIT_REG2_DDR  			0x00000008
#define INIT_REG3_DDR  			0x0000000c
#define INIT_REG4_DDR  			0x00000010


#define TGT_REG0_DDR  			0x00000020
#define TGT_REG1_DDR   			0x00000024
#define TGT_REG2_DDR  			0x00000028
#define LINK_STATE_DDR  		0x0000002c

//l2cache
#define L2CACHE_CFG_DDR 			0x00000000
#define L2CACHE_STATE_DDR  		    0x00000004
#define L2CACHE_FLUSH_MEM_DDR  	    0x00000008
#define L2CACHE_FLUSH_SET_DDR  	    0x0000000c




volatile unsigned int *irq_pend  	= NULL;
volatile unsigned int *irq_clear 	= NULL;


volatile unsigned int *init_reg0  	= NULL;
volatile unsigned int *init_reg1 	= NULL;
volatile unsigned int *init_reg2  	= NULL;
volatile unsigned int *init_reg3 	= NULL;
volatile unsigned int *init_reg4  	= NULL;

volatile unsigned int *tgt_reg0 	= NULL;
volatile unsigned int *tgt_reg1  	= NULL;
volatile unsigned int *tgt_reg2 	= NULL;
volatile unsigned int *link_state 	= NULL;
//volatile unsigned long long *init_tdfd  = NULL;



volatile unsigned int *l2cache_cfg 	 	   = NULL;
volatile unsigned int *l2cache_state  	   = NULL;
volatile unsigned int *l2cache_flush_mem   = NULL;
volatile unsigned int *l2cache_flush_set   = NULL;

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

static unsigned int val_nwrite[4] = {0};
static unsigned int val_doolbell[2] = {0};
unsigned int doorbell_date = 0;

static unsigned int dma_ddr = 0;
//static unsigned int dma_size = 512;


static long srio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret,i=0;
	
	
	//在内核中还需要将,printk的打印放在变量定义之后
	printk("enter srio_ioctl!\n");
	
	switch(cmd){
		//初始化srio并设置为NWRITE模式,启动DMA
		//初始化，配置为NWRITE
		case SRIO_NWRITE:
			printk("SRIO_NWRITE!\n");

			//flush_cache_all();

			//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);

			ret = copy_from_user(&val_nwrite, (const char*)arg, sizeof(val_nwrite)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);

			for(i=0; i<4; i++)
			{
				printk("kennel-val_nwrite[%d] = %x\n",i,*(val_nwrite+i));		
			}

			//barrier();

			*init_reg0	= *val_nwrite;
			*init_reg1	= src_phy_addr;
			*init_reg2 	= *(val_nwrite+1);
			*init_reg3 	= *(val_nwrite+2);
			*init_reg0 	= *(val_nwrite+3);	


			printk("init_reg0 = %x\n",*init_reg0);
			printk("init_reg1 = %x\n",*init_reg1);
			printk("init_reg2 = %x\n",*init_reg2);
			printk("init_reg3 = %x\n",*init_reg3);

			//dma_sync_single_for_cpu(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);
						
			
			printk("initiator NWRITE_R request:\n");  
			printk("***** board A ****\n"); 
				
			printk(">===========ioctl_nwrite_end!=============<\n");
			
			break;
		case SRIO_DOORBELL:
			printk("SRIO_DOORBELL!\n");

			ret = copy_from_user(&val_doolbell, (const char*)arg, sizeof(val_doolbell)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);

			for(i=0; i<2; i++)
			{
				printk("kennel-val_doolbell[%d] = %x\n",i,*(val_doolbell+i));		
			}


			*init_reg0	= *val_doolbell;
			*init_reg0 	= *(val_doolbell+1);


						
				
			printk(">===========ioctl_doolbell_end!=============<\n");
			
			break;
		case SRIO_DMA_DDR:
			printk("SRIO_DMA_DDR!\n");

			dma_ddr = src_phy_addr;
			
			//将dma_ddr传输到应用层，给init使用
			ret = copy_to_user((unsigned char *)arg, &dma_ddr, sizeof(dma_ddr)); //copy_to_user();
			printk("srio-dma_ddr-ret = -%d-\n",ret);


			printk("kennel-dma_ddr = %x\n",dma_ddr);	


				
			printk(">===========ioctl-SRIO_DMA_DDR-end!=============<\n");
			
			
			break;
		case SRIO_DOORBELL_DATE:
			printk("SRIO_DOORBELL_DATE!\n");
			printk("*irq_pend = %x\n"	,*irq_pend);

			//堵塞中断并等待
			while(!(*irq_pend & 0x00001000));//irq_pend[12]=1
			
			printk("kennel-tgt_reg0 = %x\n",*tgt_reg0);

			doorbell_date = *tgt_reg0;

			printk("tgt_reg2 = %x\n",*tgt_reg2);

			printk("kennel-doorbell_date = %x\n",doorbell_date);
			
			//将size传输到应用层，给target使用
			ret = copy_to_user((unsigned char *)arg, &doorbell_date, sizeof(doorbell_date)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);


				
			printk(">===========ioctl_SRIO_DOORBELL_DATE_end!=============<\n");
			
			
			break;
		case SRIO_FLUSH_DMA:
			printk("SRIO_FLUSH_DMA!\n");

			flush_cache_all();
				
			printk(">===========ioctl_SRIO_FLUSH_DMA_end!=============<\n");	
			
			break;
		case SRIO_LEON_FLUSH_DMA:
			printk("SRIO_LEON_FLUSH_DMA!\n");

		__asm__ __volatile__(".align 32\nflush\n.align 32\n");	/*iflush*/
		__asm__ __volatile__("sta %%g0, [%%g0] %0\n\t" : :
					"i"(ASI_LEON_DFLUSH) : "memory");
				
			printk(">===========ioctl_SRIO_LEON_FLUSH_DMA_end!=============<\n");	
			
			break;
		default:
			return -EINVAL;
	}	
	
	return 0;	
}


static int srio_open(struct inode * inode , struct file * filp)
{	
	
	printk("drvsrio_open\n");

	*irq_clear = 0x1000; //清除irq_pend[12]
	printk("irq is cleared\n\n");

	printk("link_state: 0x%p: 0x%x\n", link_state, *link_state);
	if(*link_state & 0x00000001) 
		  printk("link up!\n\n");		
	else 
			printk("link down!\n\n");

	//barrier();
  

printk("-----=-------\n");

/*	
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
*/
//dma_zalloc_coherent
//dma_alloc_coherent
//dma_alloc_noncoherent
//dma_alloc_writecombine 


	//连接成功后申请dma区域//|GFP_DMA|__GFP_ZERO
    src_addr = (unsigned long long *)dma_alloc_coherent(NULL, COPY_BUFF_SIZE, &src_phy_addr, GFP_KERNEL|GFP_DMA);
    if(src_addr == NULL)
    {
        printk("can't alloc buffer for src\n");
        return -ENOMEM;
    }
    printk("src_addr = %llx\n",*src_addr);
    printk("src_phy_addr = %x\n",src_phy_addr);  

	return 0;
}


//将应用层数据写入到memory中
static ssize_t srio_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	
	int ret,i;
	unsigned long long val[128] = {0};

	printk("srio_A_write\n");
	
	printk("*irq_pend = %x\n"	,*irq_pend);

	//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);

	//printk("(*target_reg0 & 0x3c000000)/4000000 = %x\n"	,(*target_reg0 & 0x3c000000)/4000000);
	//nwrite事务的写、

printk("--1--kennel-l2cache_cfg = %x\n",*l2cache_cfg);
printk("--1--kennel-l2cache_flush_mem = %x\n",*l2cache_flush_mem);
	//关闭l2c
	//*l2cache_cfg &= 0x7fffffff;
	//设置写回
	*l2cache_flush_mem |= 0xf;

printk("--2--kennel-l2cache_cfg = %x\n",*l2cache_cfg);
printk("--2--kennel-l2cache_flush_mem = %x\n",*l2cache_flush_mem);
	
	
	mutex_lock(&my_mutex);

	ret = copy_from_user(&val, buf, count); //copy_to_user();
	printk("srio-nwriter-ret = -%d-\n",ret);

	
	for(i=0; i<count/8; i++)
	{
		printk("kennel-write-val[%d] = %llx\n",i,val[i]);		
	}	

/*
	if(memcmp(src_addr, val, 512) == 0)
	{
		printk("MEM_WRITE_DMA OK\n");
	}
	else
	{
		printk("MEM_WRITE_DMA ERROR\n");
	}
*/
/*
	for(i=0; i<count/8; i++)
	{
		*(src_addr+i*sizeof(val[0]))=*(val+i);			
	}
*/
//*src_addr = *val;
	//memcpy(val,src_addr,strlen(val)+1);


	for(i = 0; i < count/8; i++)
	{
		//printk("before: dst_addr[%d] = %x,src_addr[%d] = %x\n",n,dst_addr[n],n,src_addr[n]);

		src_addr[i] = val[i];

		//printk("later: dst_addr[%d] = %x,src_addr[%d] = %x\n",n,dst_addr[n],n,src_addr[n]);

	}

	//dma_sync_single_for_cpu(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);
	//memset(src_addr, 0x55, 512);


//flush_cache_all();
//leon_flush_cache_all();


//printk("%s,%d\n",__func__,__LINE__);

	mutex_unlock(&my_mutex);	

	//flush_cache_all();
	//leon_flush_cache_all();


	
    return 0;
}

static ssize_t srio_read(struct file *file,char __user *buf, size_t count, loff_t * ppos)
{
	volatile unsigned long long *val = NULL;
	int ret,i;
	unsigned long long val_buf[128] = { 0 };
	printk("srio_B_read\n");
	printk("irq_pend = %x\n",*irq_pend);
	//while(!(*irq_pend & 0x1000));

	//flush_cache_all();

	//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_FROM_DEVICE);

/*
				dma_sync_single_for_cpu(greth->dev,
							dma_addr,
							pkt_len,
							DMA_FROM_DEVICE);

		dma_sync_single_for_device(greth->dev, dma_addr, MAX_FRAME_SIZE, DMA_FROM_DEVICE);

*/

	val = src_addr;	

	//(*tgt_rlr/8)
	for(i=0; i<count/8; i++)
	{
		val_buf[i] = (unsigned long long)*(val+i);
		printk("kennel-read-val_buf[%d] = %llx\n",i,val_buf[i]);	
	
	}

	printk("tgt_reg0 =  0x%x\n", ioread32(tgt_reg0));
	printk("tgt_reg1 =  0x%x\n", ioread32(tgt_reg1));
	printk("tgt_reg2 =  0x%x\n", ioread32(tgt_reg2));
	

	mutex_lock(&my_mutex);

	ret = copy_to_user(buf, &val_buf, count);
	printk("srio-read-ret = -%d-\n",ret);

	mutex_unlock(&my_mutex);

	//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_FROM_DEVICE);

	*irq_clear = 0x1000; //清除irq_pend[12]
	printk("irq is cleared\n\n");		

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
	irq_pend  	  	= (volatile unsigned int *)ioremap(IRQ_PEND_BASE_DDR, 32);
	irq_clear 	  	= (volatile unsigned int *)ioremap(IRQ_CLEAR_BASE_DDR, 32);



	init_reg0 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+INIT_REG0_DDR, 32);
	init_reg1 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+INIT_REG1_DDR, 32);
	init_reg2  	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+INIT_REG2_DDR , 32);
	init_reg3 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+INIT_REG3_DDR, 32);
	init_reg4  	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+INIT_REG4_DDR , 32);

	tgt_reg0 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+TGT_REG0_DDR	, 32);
	tgt_reg1 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+TGT_REG1_DDR	, 32);
	tgt_reg2 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+TGT_REG2_DDR , 32);
	link_state 	= (volatile unsigned int *)ioremap(TGT_INIT_BASE_DDR+LINK_STATE_DDR , 32);
	//init_tdfd  		= (volatile unsigned long long *)ioremap(INIT_DATA_BASE_DDR+INIT_TGT_TDFD_DDR, 0xfff);
	//init_rdfd  		= (volatile unsigned long long *)ioremap(INIT_DATA_BASE_DDR+INIT_TGT_RDFD_DDR, 0xfff);

	l2cache_cfg 	 	= (volatile unsigned int *)ioremap(L2CACHE_BASE_DDR+L2CACHE_CFG_DDR 	 , 32);
	l2cache_state  	 	= (volatile unsigned int *)ioremap(L2CACHE_BASE_DDR+L2CACHE_STATE_DDR  	 , 32);
	l2cache_flush_mem 	= (volatile unsigned int *)ioremap(L2CACHE_BASE_DDR+L2CACHE_FLUSH_MEM_DDR, 32);
	l2cache_flush_set 	= (volatile unsigned int *)ioremap(L2CACHE_BASE_DDR+L2CACHE_FLUSH_SET_DDR, 32);



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

	iounmap(init_reg0);
	iounmap(init_reg1);
    iounmap(init_reg2);
	iounmap(init_reg3);
    iounmap(init_reg4);

	iounmap(tgt_reg0);
    iounmap(tgt_reg1);
	iounmap(tgt_reg2);

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
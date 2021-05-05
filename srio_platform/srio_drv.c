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

#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/kdev_t.h>

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/of_irq.h>




#define SRIO_MINOR 0

//ioctl命令
#define SRIO_DMA _IO('s',0)
#define SRIO_NWRITE _IO('s',1)
#define SRIO_DOORBELL _IO('s',2)
#define SRIO_DMA_DDR _IO('s',3)
#define SRIO_DOORBELL_DATE _IO('s',4)     





size_t buff_size = (1024*8)*2;						//申请DMA空间2MB,(1024*8)*2

unsigned long long *src_vir_addr;		//源虚拟地址
dma_addr_t src_phy_addr;					//源物理地址


//资源地址
struct resource *srio_res;
//IO映射
void __iomem *srio_base = NULL;

/*---------------------配置srio寄存器--------------------*/
#define INIT_REG0_DDR   		0x00000000
#define INIT_REG1_DDR   		0x00000004
#define INIT_REG2_DDR  			0x00000008
#define INIT_REG3_DDR  			0x0000000c
#define INIT_REG4_DDR  			0x00000010

#define TGT_REG0_DDR  			0x00000020
#define TGT_REG1_DDR   			0x00000024
#define TGT_REG2_DDR  			0x00000028
#define LINK_STATE_DDR  		0x0000002c





volatile unsigned int *irq_pend  	= NULL;
volatile unsigned int *irq_clear 	= NULL;





//定义mutex,互斥锁
struct mutex my_mutex;


u32 val_nwrite[4] = {0};
unsigned int val_doolbell[2] = {0};
u32 doorbell_date = 0;

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

			//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);

			ret = copy_from_user(&val_nwrite, (const char*)arg, sizeof(val_nwrite)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);

			for(i=0; i<4; i++)
			{
				printk("kennel-val_nwrite[%d] = %x\n",i,*(val_nwrite+i));		
			}

			//barrier();

			iowrite32(*val_nwrite,srio_base + INIT_REG0_DDR);
			iowrite32(src_phy_addr,srio_base + INIT_REG1_DDR);
			iowrite32(*(val_nwrite+1),srio_base + INIT_REG2_DDR);
			iowrite32(*(val_nwrite+2),srio_base + INIT_REG3_DDR);
			iowrite32(*(val_nwrite+3),srio_base + INIT_REG0_DDR);



			//printk("srio_base + INIT_REG0_DDR = %x\n",*(srio_base + INIT_REG0_DDR));
			//printk("srio_base + INIT_REG1_DDR = %x\n",*(srio_base + INIT_REG1_DDR));
			//printk("srio_base + INIT_REG2_DDR = %x\n",*(srio_base + INIT_REG2_DDR));
			//printk("srio_base + INIT_REG3_DDR = %x\n",*(srio_base + INIT_REG3_DDR));

			//dma_sync_single_for_cpu(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_FROM_DEVICE);
						
			
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


			iowrite32(*val_doolbell,srio_base + INIT_REG0_DDR);
			iowrite32(*(val_doolbell+1),srio_base + INIT_REG0_DDR);


						
				
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
			
			//printk("kennel-srio_base + TGT_REG0_DDR = %x\n",*(srio_base + TGT_REG0_DDR));


			iowrite32(doorbell_date,srio_base + TGT_REG0_DDR);


			//printk("srio_base + TGT_REG2_DDR = %x\n",*(srio_base + TGT_REG2_DDR));

			printk("kennel-doorbell_date = %x\n",doorbell_date);
			
			//将size传输到应用层，给target使用
			ret = copy_to_user((unsigned char *)arg, &doorbell_date, sizeof(doorbell_date)); //copy_to_user();
			printk("srio_ioctl_nread-ret = -%d-\n",ret);


				
			printk(">===========ioctl_SRIO_DOORBELL_DATE_end!=============<\n");
			
			
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


	iowrite32(*val_doolbell,srio_base + INIT_REG0_DDR);

	//printk("LINK_STATE_DDR: 0x%p: 0x%x\n", LINK_STATE_DDR, *LINK_STATE_DDR);
	if(ioread32(srio_base + LINK_STATE_DDR) & 0x00000001) 
		  printk("link up!\n\n");		
	else 
			printk("link down!\n\n");

	//barrier();
  

printk("-----=-------");



//dma_zalloc_coherent
//dma_alloc_coherent
//dma_alloc_noncoherent
//dma_alloc_writecombine 

	//连接成功后申请dma区域
    src_vir_addr = (unsigned long long *)dma_alloc_coherent(NULL, buff_size, &src_phy_addr, GFP_KERNEL|GFP_DMA);
    if(src_vir_addr == NULL)
    {
        printk("can't alloc buffer for src\n");
        return -ENOMEM;
    }
    printk("src_vir_addr = %llx\n",*src_vir_addr);
    printk("src_phy_addr = %x\n",src_phy_addr);  







	return 0;
}

static ssize_t srio_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	
	int ret,i;
	unsigned long long val[128] = {0};

	printk("srio_A_write\n");
	
	printk("*irq_pend = %x\n"	,*irq_pend);

	//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);

	//printk("(*target_reg0 & 0x3c000000)/4000000 = %x\n"	,(*target_reg0 & 0x3c000000)/4000000);
	//nwrite事务的写
	mutex_lock(&my_mutex);

	ret = copy_from_user(&val, buf, count); //copy_to_user();
	printk("srio-nwriter-ret = -%d-\n",ret);

	
	for(i=0; i<count/8; i++)
	{
		printk("kennel-write-val[%d] = %llx\n",i,val[i]);		
	}	

/*
	if(memcmp(src_vir_addr, val, 512) == 0)
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
		*(src_vir_addr+i*sizeof(val[0]))=*(val+i);			
	}
*/
//*src_vir_addr = *val;
	//memcpy(val,src_vir_addr,strlen(val)+1);


	for(i = 0; i < count/8; i++)
	{
		//printk("before: dst_addr[%d] = %x,src_vir_addr[%d] = %x\n",n,dst_addr[n],n,src_vir_addr[n]);

		src_vir_addr[i] = val[i];

		//printk("later: dst_addr[%d] = %x,src_vir_addr[%d] = %x\n",n,dst_addr[n],n,src_vir_addr[n]);

	}

	//dma_sync_single_for_cpu(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_FROM_DEVICE);
	//memset(src_vir_addr, 0x55, 512);

printk("%s,%d\n",__func__,__LINE__);
	mutex_unlock(&my_mutex);	

	
    return 0;
}

static ssize_t srio_read(struct file *file,char __user *buf, size_t count, loff_t * ppos)
{
	printk("srio_B_read\n");
	printk("irq_pend = %x\n",*irq_pend);
	while(!(*irq_pend & 0x1000));

	//dma_sync_single_for_device(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_TO_DEVICE);

	//printk("TGT_REG0_DDR =  0x%x\n", ioread32(TGT_REG0_DDR));
	//printk("TGT_REG1_DDR =  0x%x\n", ioread32(TGT_REG1_DDR));
	//printk("TGT_REG2_DDR =  0x%x\n", ioread32(TGT_REG2_DDR));

	//dma_sync_single_for_cpu(srio_device, src_phy_addr,COPY_BUFF_SIZE,  DMA_FROM_DEVICE);

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
static struct file_operations srio_ops = {
	.owner  =   THIS_MODULE,    //这是一个宏，推向编译模块时自动创建的__this_module变量
	.open   =   srio_open,     
	.write  =   srio_write,
	.read  	=   srio_read,
	.unlocked_ioctl = srio_ioctl,//ioctl接口
	.release = srio_release,//对应用户close接口		
};



//分配初始化miscdevice
struct miscdevice srio_dev = {
	.minor = MISC_DYNAMIC_MINOR,//系统分配次设备号
	.name = "srio",//设备文件名
	.fops = &srio_ops,//操作函数集合
};



//中断处理函数
irqreturn_t srio_interrupt(int irq, void *dev_id)
{
	printk("hello irq!\n");

	return IRQ_HANDLED;//处理成功
}


//srio设备的probe函数实现
static int srio_probe(struct platform_device *pdev)
{
	int ret,irq,err;
	struct device_node *np = pdev->dev.of_node;

	printk("match srio open srio_probe");
	
	//注册miscdevice
	ret = misc_register(&srio_dev);
	if(ret<0){
		printk("misc_register failed!\n");
	}

	//获取srio的资源
	srio_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(!srio_res){
		printk("platform_get_srio_resource failed!\n");
	}

	//映射IO内存
	srio_base = devm_ioremap_resource(&pdev->dev, srio_res);
	if (IS_ERR(srio_base))
		return PTR_ERR(srio_base);

	//中断
	irq = irq_of_parse_and_map(np, 0);
	if (!irq) {
		dev_err(&pdev->dev, "no irq found\n");
	}

/*
	ret = request_irq(gpio_to_irq(btn_info[i].gpio), //中断号
						btn_handler, //中断处理函数
						IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, //中断标志,包括触发方式----- 上升下降沿触发
						"SRIO12", //中断名称
						&btn_info[i]);//传递给中断处理函数的参数
*/

	err = request_irq(12, srio_interrupt, IRQF_SHARED,
			  "SRIO12", NULL);
	if (err){
		printk("request irq failed");
	}


	printk("srio_probe end!\n");

	return ret;     
}



int srio_remove(struct platform_device *pdev)
{
	//解除IO映射
	iounmap(srio_base);

	//注销miscdevice	
	misc_deregister(&srio_dev);

	return 0;
}


static const struct of_device_id srio_of_match[] = {
	{.name = "YDDD_SRIO"},
	{.name = "19_09"},
	{},
};

MODULE_DEVICE_TABLE(of,srio_of_match);


static struct platform_driver srio_drv = {
	.driver = {
		.name = "srio",
		.of_match_table = srio_of_match,
	},
	.probe = srio_probe,
	.remove = srio_remove,
};

module_platform_driver(srio_drv);


MODULE_AUTHOR("Qianer");	//作者
MODULE_DESCRIPTION("SRIO moudle dirver");	//模块描述
MODULE_VERSION("v1.0");		//版本
MODULE_LICENSE("GPL");		//GPL模块许可证
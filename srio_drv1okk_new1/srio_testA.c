#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

//ioctl命令
#define SRIO_DMA _IO('s',0)
#define SRIO_NWRITE _IO('s',1)
#define SRIO_DOORBELL _IO('s',2)
#define SRIO_DMA_DDR _IO('s',3)
#define SRIO_DOORBELL_DATE _IO('s',4)  

//申请DMA空间2MB
#define COPY_BUFF_SIZE (1024*8)*2
#define COPY_SIZE 512

int main(int argc, char **argv)
{
	unsigned long long fd;
	int i = 0;
	char ch = 0;
	unsigned long long w_val[256] = { 0 };
	unsigned long long r_val[256] = { 0 };
	//配置为nwrite
	unsigned int val_nwrite[4] = {0x05000000,512,0,0x85000000};
	unsigned int val_doolbell[2] = {0};

	unsigned int dma_ddr = 0;

	//1.打开srio0设备，可读可写打开
	fd = open("/dev/srio", O_RDWR);
	if (fd < 0)
	{
		printf("fd_srio can't open!\n");
	}
	//初始化写入数据
	for(i=0; i<(COPY_SIZE/8); i++)
	{
		w_val[i] = 0x1111111122222222+i;
		printf("user-write-w_val[%d] = %llx\n",i,w_val[i]);		
	}

	printf("COPY_SIZE = %d\n",COPY_SIZE);	

	while(1){
		ch = getchar();
		getchar();

		if(ch=='q')
			break;

		switch(ch){
			case '1':
				//1.往memory里面写数据
				write(fd, &w_val, COPY_SIZE);
				break;
			case '2':
				//获取到kernel的DMA首地址
				ioctl(fd,SRIO_DOORBELL_DATE,&val_doolbell);
				dma_ddr = dma_ddr & ((val_doolbell[0] & 0x00ffff00) >> 8);
				break;
			case '3':
				//获取到kernel的DMA首地址
				ioctl(fd,SRIO_DOORBELL_DATE,&val_doolbell);
				dma_ddr = dma_ddr & ((val_doolbell[0] & 0x00ffff00) << 8);
				val_nwrite[2] = dma_ddr;
				break;
			case '4':
				//3.配置为NWRITE，启动dma
				ioctl(fd,SRIO_NWRITE,&val_nwrite);
				break;
			default:
				printf("error input!\n");
				break;
		}
		sleep(1);	
	}
	//4.关闭设备
	close(fd);

    return 0;
}

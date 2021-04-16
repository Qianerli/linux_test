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

#define COPY_SIZE 512

int main(int argc, char **argv)
{
	unsigned long long fd;
	int i = 0;
	char ch = 0;
	unsigned long long w_val[256] = { 0 };
	unsigned long long r_val[256] = { 0 };

	//配置为doorbell
	unsigned int val_doorbell[2] = {0x0a000000,0x8a000000};
	unsigned int low = 0;
	unsigned int high = 0;

	unsigned int dma_ddr = 0;


	//1.打开spw0设备，可读可写打开
	fd = open("/dev/srio", O_RDWR);
	if (fd < 0)
	{
		printf("fd_srio can't open!\n");
	}	

	
	while(1){
		ch = getchar();
		getchar();

		if(ch=='q')
			break;

		switch(ch){
			case '1':
				//获取到kernel的DMA首地址
				ioctl(fd,SRIO_DMA_DDR,&dma_ddr);
				printf("dma_ddr = %x\n",dma_ddr);
				break;
			case '2':
				//2.配置为DOORBELL，发送
				val_doorbell[0] = val_doorbell[0] & 0xff000000;		
				val_doorbell[0] = val_doorbell[0] | ((dma_ddr & 0x0000ffff)<<8);
				val_doorbell[1] = val_doorbell[1] & 0xff000000;		
				val_doorbell[1] = val_doorbell[1] | ((dma_ddr & 0x0000ffff)<<8);

				ioctl(fd,SRIO_DOORBELL,&val_doorbell);
				break;
			case '3':
				//3.配置为DOORBELL，发送
				val_doorbell[0] = val_doorbell[0] & 0xff000000;
				val_doorbell[0] = ((dma_ddr & 0xffff0000)>>8) | val_doorbell[0];
				val_doorbell[1] = val_doorbell[1] & 0xff000000;
				val_doorbell[1] = ((dma_ddr & 0xffff0000)>>8) | val_doorbell[1];

				ioctl(fd,SRIO_DOORBELL,&val_doorbell);
				break;
			case '4':
				//3.测试nread读写
				//3.1 读
				read(fd, &r_val, COPY_SIZE);
				/*
				for(i=0; i<COPY_SIZE/8; i++)
				{
					printf("user-read-r_val[%d] = %llx\n",i,r_val[i]);		
				}
				*/
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
